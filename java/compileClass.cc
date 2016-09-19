/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/**
   A simple application that produces a class file, and a C++ JNI jumptable
*/

#include <stdio.h>
#include <dlfcn.h>
#include <iostream>
#include <fstream>
#include "object.h"
#include "JNIEx.h"
#include "javaClass.h"
#include "javaClass_serialisation.h"
#include "javaClass.cd"
#include "classdesc_epilogue.h"

using namespace classdesc;
using namespace std;

namespace classdesc
{
  JNIRegistry& JNIexporterRegistry()
  {
    static JNIRegistry theRegistry;
    return theRegistry;
  }
}

namespace
{
// return a list of JNI argument types matching signature
// last item is return type
std::vector<std::string> JNI_arglist(const std::string& sig)
{
  std::vector<std::string> ret;
  for (std::string::const_iterator c=sig.begin(); c!=sig.end(); c++)
    {
      switch (*c)
        {
        case 'Z':  ret.push_back("jboolean"); break;
        case 'B':  ret.push_back("jbyte"); break;
        case 'C':  ret.push_back("jchar"); break;
        case 'S':  ret.push_back("jshort"); break;
        case 'I':  ret.push_back("jint"); break;
        case 'J':  ret.push_back("jlong"); break;
        case 'F':  ret.push_back("jfloat"); break;
        case 'D':  ret.push_back("jdouble"); break;
        case 'V':  ret.push_back("void"); break;
        case 'L':  
          if (sig.substr(c-sig.begin(),18)=="Ljava/lang/String;")
            ret.push_back("jstring");
          else
            ret.push_back("jobject"); 
          while (*c!=';') ++c;
          break;
        case '[': c++;
          switch (*c)
            {
            case 'Z':  ret.push_back("jbooleanArray"); break;
            case 'B':  ret.push_back("jbyteArray"); break;
            case 'C':  ret.push_back("jcharArray"); break;
            case 'S':  ret.push_back("jshortArray"); break;
            case 'I':  ret.push_back("jintArry"); break;
            case 'J':  ret.push_back("jlongArray"); break;
            case 'F':  ret.push_back("jfloatArray"); break;
            case 'D':  ret.push_back("jdoubleArray"); break;
            case 'L':  ret.push_back("jobjectArray");
              while (*c!=';') ++c;
              break;
            }     
        case ')': case '(': break; 
        default:
          throw sig_error();
        }
    }
  return ret;
}

class write_JNI_t: public javaClass_t
{
  std::string class_name;
  std::ostream& out;
public:
  write_JNI_t(std::string class_name, std::ostream& out): 
    class_name(class_name), out(out) {}
  void add_functor(const std::string& name, Functional_ptr f, const std::string& sig)
  {
    std::string nm = name.substr(name.rfind(".")+1);
    vector<std::string> argtypes = JNI_arglist(sig);
    out << "extern \"C\" "<<argtypes.back()<<
      " Java_"<<class_name<<"_"<<nm<<
      "(JNIEnv* env, jobject jthis";
    for (size_t i=0; i<argtypes.size()-1; ++i)
      out << ", "<<argtypes[i]<<" a"<<i;
    out << ")\n"<<
      "{\n"<<
      "  try\n"<<
      "  {"<<
      "    std::string this_=cppObject(env,jthis);\n"<<
      "    static Functional_ptr f=method_registry[this_+\"."<<nm<<"\"];\n"<<
      "    ArgVector args("<<argtypes.size()-1<<");\n";
    for (size_t i=0; i<argtypes.size()-1; ++i)
      out << "    args["<<i<<"]=Arg(env,a"<<i<<");\n";
    out << "    RetVal ret;\n" <<
      "    (*f)(ret,args);\n";
    if (argtypes.back()!="void")
      out << "    return ret->get_"<<argtypes.back()<<"(env);\n";
    out << "  }\n"
      "  catch (std::exception& ex)\n"
      "  {\n"
      "   env->ThrowNew(env->FindClass(\"java/lang/Throwable\"),ex.what());\n";
    if (argtypes.back()!="void")
      out <<"   return "<<argtypes.back()<<
        "(); //never executed, but shuts -Werror up\n";
    out <<"  }\n"
      "}\n\n";
  }
};


  void write_JNI(const JNIexporterBase& object)
  {
    ofstream outfile((object.name+".cc").c_str());
    write_JNI_t cf(object.name, outfile);
    // push on placeholder for a reference to super's constructor
    cf.constant_pool.push_back(cp_info());

    outfile << "#include \""<<object.name<<".h\"\n";
    outfile << "#include \"JNIEx.h\"\nusing namespace classdesc;\n";
    outfile << "#include \"classdesc_epilogue.h\"\n";
    // Output C++ JNI code, and reflect the object into the javaClass structure
    object.exportClass(cf);
    // add the auto() method (C++ JNI constructor code)
    cf.addMethod("auto","()V");
    
    // Output initialiser code. auto is chosen because it is a keyword
    // in C++, but not Java, so there is no possibility of conflict
    outfile << "extern \"C\" void Java_"<<object.name<<
      "_auto(JNIEnv *env, jobject obj)\n" <<
      "{registerCppObject<"<<object.name<<">(env,obj);}";
    
    //set up super_class field as first entry in constant pool
    cf.super_class=cf.constant_pool.size();
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_Class,u2(cf.constant_pool.size()+1)));
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8,std::string("CppObject")));
    
    //set up this_class field
    cf.this_class=cf.constant_pool.size();
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_Class,u2(cf.constant_pool.size()+1)));
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8,object.name));
    
    cf.makeNative();
    
    // add an empty default constructor
    cf.addMethod("<init>","()V");
    cf.methods.back().access_flags = JVM_ACC_PUBLIC;
    // add a name & type reference for default constructors
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_NameAndType,
                                       NameAndTypeInfo(cf.constant_pool.size()-2,
                                                       cf.constant_pool.size()-1)
                                       ));
    //fill in super constructor's default reference
    cf.constant_pool[1].set(JVM_CONSTANT_Methodref, 
                            Ref(cf.super_class, cf.constant_pool.size()-1));
    
    // add the constructor byte code
    attribute_info ai;
    ai.attribute_name_index = cf.constant_pool.size();
    cf.constant_pool.push_back(cp_info(JVM_CONSTANT_Utf8, std::string("Code")));
    unsigned char code[]={0,1,0,1,0,0,0,5,0x2a,0xb7,0,1,0xb1,0,0,0,
                          1,0,7,0,0,0,6,0,1,0,0,0,1}; //"Return"
    ai.info.insert(ai.info.end(), code, code+sizeof(code));
    cf.methods.back().attributes.push_back(ai);
    
    // output the compiled Java class to a class file
    pack_t buf((object.name+".class").c_str(),"w");
    buf << static_cast<ClassFile&>(cf);
  }
  
}

int main(int argc, char* argv[])
{
  // load up any named dynamic libraries
  for (int i=1; i<argc; ++i)
    if (!dlopen(argv[i], RTLD_NOW))
      cerr << dlerror()<<endl;

  // export C++ JNI and Java class files for all registered objects
  for (JNIRegistry::const_iterator obj=JNIexporterRegistry().begin(); 
       obj!=JNIexporterRegistry().end(); ++obj)
    {
      cout << "writing support for "<<(*obj)->name<<endl;
      write_JNI(**obj);
    }
  return 0;
}
  

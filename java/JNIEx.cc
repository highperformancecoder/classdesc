/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "javaClass.h"
#include "javaClass_serialisation.h"
#include "javaClass.cd"
#include "classdesc_epilogue.h"

#include "JNIEx.h"
#include <jni.h>

#include <fstream>

using namespace classdesc;
using classdesc::Arg;
using namespace std;

std::map<std::string, Functional_ptr> method_registry;

CleanupContainer classdesc::cleanupContainer;



template <class JavaClass>
void create_java_class(JavaClass& cf, const classdesc::string& object_name)
{
}


//extern "C" jbyteArray Java_JNIInterface_getExampleInterface(JNIEnv* env, jobject loader)
//{
//  assert(loader);
//  register_java_class cf("ExampleIntf");
//  create_java_class(cf,"ex");
//  cf.makeInterface();
//  pack_t buf;
//  buf << static_cast<ClassFile&>(cf);
//
//  jbyteArray cl = env->NewByteArray(buf.size());
//  assert(cl);
//  env->SetByteArrayRegion(cl,0,buf.size(),(jbyte*)buf.data());
//  return cl;
//}

extern "C" jobject Java_JNIInterface_call(JNIEnv* env, jobject loader, jstring name, jobjectArray args)
{
  try
    {
      RetVal ret;

      const char *c_name = env->GetStringUTFChars(name, NULL); 
//
//  for (std::map<std::string, Functional_ptr>::iterator i=method_registry.begin(); i!=method_registry.end(); ++i)
//    cout << i->first<<endl;


      assert(method_registry.count(c_name));
      ArgVector cArgs(env,args);
      
      (*method_registry[c_name])(ret, cArgs);
      env->ReleaseStringUTFChars(name, c_name);

      assert(ret);
      return ret->get_jobject(env);
    }
  catch (std::exception& ex)
    {
      env->ThrowNew(env->FindClass("java/lang/Throwable"),ex.what());
    }
  return jobject();
}


namespace classdesc
{
#pragma weak JNIexporterRegistry
  JNIRegistry& JNIexporterRegistry()
  {
    static JNIRegistry theRegistry;
    return theRegistry;
  }


}

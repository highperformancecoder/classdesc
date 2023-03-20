/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "javaClass_base.h"
#include <sstream>
extern std::map<std::string, classdesc::Functional_ptr> method_registry;

namespace classdesc
{
  struct JNIexporterBase
  {
    std::string name;
    virtual void exportClass(javaClass_t& cf) const=0;
  };

  template <class T>
  struct JNIexporter: public JNIexporterBase
  {
    T archetype;
    void exportClass(javaClass_t& cf) const {javaClass(cf, name, archetype);}
  };

  // this is a singleton to get around library load order issues
  typedef std::vector<shared_ptr<JNIexporterBase> > JNIRegistry;
  JNIRegistry& JNIexporterRegistry();

  template <class T>
  struct JNIRegister
  {
    JNIRegister(const std::string& name) {
      shared_ptr<JNIexporterBase> object(new JNIexporter<T>);
      object->name=name;
      JNIexporterRegistry().push_back(object);
      std::cout << "registering "<<name << std::endl;
    }
  };

  //void write_JNI(const JNIexporterBase& object);

  /// a base class to add a virtual destructor to supplied types t
  /// allow for cleanup
  struct BaseForCleanup
  {
    virtual ~BaseForCleanup() {}
  };
  template <class T>
  struct Cleanup: public T, public BaseForCleanup {};

  /// a container storing all allocated C++ objects, for cleanup at DLL unload
  typedef std::vector<shared_ptr<BaseForCleanup> > CleanupContainer;
  extern CleanupContainer cleanupContainer;

  struct register_java_class: public javaClass_t
  {
    void add_functor(const std::string& name, Functional_ptr f, const std::string& sig)
    {
      method_registry.insert(make_pair(name,f));
    }
};



  /// initialise a Java CppObject with a newly created C++ object of type T
  template <class T>
  void registerCppObject(JNIEnv *env, jobject obj)
  {
    try
      {
        jclass clazz=env->GetObjectClass(obj);
        assert(clazz);
        // register is chosen because it is a C++ keyword, so won't
        // conflict with any method names
        jfieldID fld=env->GetFieldID(clazz,"register","Ljava/lang/String;");
        cleanupContainer.push_back(shared_ptr<BaseForCleanup>(new Cleanup<T>));
        std::ostringstream objName;
        objName << std::hex << cleanupContainer.back().get();
        std::string objNameStr=objName.str();
        std::vector<jchar> jname(objNameStr.begin(), objNameStr.end());
        env->SetObjectField(obj,fld,env->NewString(jname.data(), jname.size()));
        // register the objects instance variables and methods with the
        // method registry
        register_java_class jc;
        javaClass(jc,objName.str(),dynamic_cast<T&>(*cleanupContainer.back()));
      }
    catch (std::exception& ex)
      {
        env->ThrowNew(env->FindClass("java/lang/Throwable"),ex.what());
      }
  }

  /// Obtain the name of the C++ object pointed to by a Java CppObject
  inline std::string cppObject(JNIEnv *env, jobject obj)
  {
    jfieldID fld=env->GetFieldID(env->GetObjectClass(obj),"register",
                          "Ljava/lang/String;");
    jstring jname=reinterpret_cast<jstring>(env->GetObjectField(obj,fld));
    const jchar* jnameptr=env->GetStringChars(jname,NULL);
    std::string name(jnameptr, jnameptr+env->GetStringLength(jname));
    env->ReleaseStringChars(jname,jnameptr);
    return name;
  }


  template <class M> struct typeof_member;
  template <class type, class C> struct typeof_member<type (C::*)>
  { typedef type T;};

  /// specialise for instance variables to use getter_setters
  template <class O, class M> typename
  enable_if<is_member_object_pointer<M>, 
            getter_setter<typename typeof_member<M>::T>
            >::T
  make_functional(O& obj, const M& member) 
  {
    return getter_setter<typename typeof_member<M>::T>(obj.*member);
  }

  /// specialise for member function variables 
  template <class O, class M> typename
  enable_if<is_member_function_pointer<M>, 
            Functional<functional::bound_method<O,M> > >::T
  make_functional(O& obj, const M& member) 
  {
    return Functional<functional::bound_method<O,M> >(bind1st(member, obj));
  }


}

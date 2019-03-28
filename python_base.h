/*
  @copyright Russell Standish 2018
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for pythonDetails.
*/

#ifndef PYTHON_BASE_H
#define PYTHON_BASE_H
#include "classdesc.h"

#include "function.h"
#include <boost/mpl/vector.hpp>

/*
  For all types, maintain a vector of polymorphic class objects
  use lazy instantiation 

  std::vector<shared_ptr<BaseObj>> classes;
  class_<T>& getClass<T>()
  {
     static size_t id=classes.size();
     if (id==classes.size())
        classes.push_back(shared_ptr<BaseObj>(new class_<T>(typeName<T>())));
     return dynamic_cast<class_<T>&>(*classes[id]);
  }
*/


namespace classdesc
{
  class python_t;

  template <class T>
  struct ClassdescEnabledPythonType:
    public And<And<And<is_class<T>, Not<is_container<T> > >, Not<is_associative_container<T> > >, Not<is_enum<T> > > {};

  /// types that have a primitive representation in Python
  template <class T>
  struct PythonBasicType:
    public Or<is_fundamental<T>, is_string<T> > {};
  
  // objectless calls
  // classdesc generated
  template <class T, class Base=T>
  typename enable_if<ClassdescEnabledPythonType<Base>,void>::T
  python(python_t& p, const string& d);


  // anything that is neither a class nor enum
  template <class T>
  typename enable_if<And<Not<is_class<T> >,Not<is_enum<T> > >,void>::T
  python(python_t&, const string&) {}

  namespace pythonDetail
  {
    using namespace classdesc::functional;

    template <class M> struct MemberType {typedef M T;};
    template <class U, class V>
    struct MemberType<U (V::*)>
    {
      typedef U T;
    };
    template <class U>
    struct MemberType<U*>
    {
      typedef U T;
    };

    template <class R, int> struct SigArg;

    template <class F> struct SigArg<F,0>
    {
      typedef boost::mpl::vector<> T;
    };

    template <class F> struct SigArg<F,1>
    {
      typedef boost::mpl::vector<typename Arg<F,Arity<F>::V>::T> T;
    };

    template <class F,int N> struct SigArg
    {
      typedef typename boost::mpl::push_front<
        typename SigArg<F,N-1>::T,
        typename Arg<F,Arity<F>::V-N+1>::T
        >::type T;
    };

    template <class F> struct SigFun
    {
      typedef typename boost::mpl::push_front<
        typename SigArg<F,Arity<F>::V>::T,
        typename Return<F>::T
        >::type T;
    };
  
    template <class F>
    struct SigObj
    {
      typedef typename classdesc::pythonDetail::SigFun<decltype(&F::operator())>::T T;
    };

    template <class T> struct Sig: public std::conditional<is_class<T>::value, SigObj<T>, SigFun<T>>::type {};

    template <class T>
    size_t len(T& x) {return x.size();}
    
    template <class T, class U>
    typename enable_if<is_assignable<T&,U>, void>::T
    assign(T& x, const U& y) {x=y;}

    template <class T, class U>
    typename enable_if<Not<is_assignable<T&,U> >, void>::T
    assign(T& x, const U& y) {throw std::runtime_error("assignment not supported between "+typeName<U>()+" and "+typeName<T>());}

    // registerClass is a MixIn because terminating rank differs from that of NewArrayGet
    template <class T, int rank>
    struct ArrayGetRegisterClass
    {static void registerClass(python_t& p);};

    template <class T>
    struct ArrayGetRegisterClass<T,0>
    {static void registerClass(python_t& p) {python<T>(p,"");}};

    template <class T, int rank> struct ArrayGetReturn;
    
    template <class T, int rank>
    struct ArrayGet: public ArrayGetRegisterClass<T,rank>
    {
      static_assert(rank==std::rank<T>::value);
      T* x;
      ArrayGet(): x(0) {}
      ArrayGet(T& x): x(&x) {}
      typedef typename ArrayGetReturn<T,rank>::T L; 
      L get(size_t i) const
      {return L((*x)[i]);}
      void set(size_t i, const L& v) {assign((*x)[i],v);}
    };

    template <class U, int rank> struct ArrayGetReturn
    {typedef ArrayGet<typename std::remove_extent<U>::type,rank-1> T;};
    template <class U> struct ArrayGetReturn<U,1>
    {typedef typename std::remove_extent<U>::type T;};

   
    template <class T, class M>
    struct ArrayMemRef
    {
      typedef typename MemberType<M>::T MT;
      static constexpr size_t rank=std::rank<MT>::value;
      static_assert(rank>0);
      M m;
      ArrayMemRef(M m): m(m) {}
      typedef ArrayGet<MT,rank> L; 

      L operator()(T& o) const 
      {return L(o.*m);}
    };

    template<class T, class M>
    struct ArrayMemRefSetItem
    {
      M m;
      ArrayMemRefSetItem(M m): m(m) {}
      typedef typename std::remove_all_extents<typename MemberType<M>::T>::type V; 
      void operator()(T& o, size_t i, V& v) const 
      {assign((o.*m)[i], v);}
    };
  
    template <class T, class M>
    size_t arrayMemLen(const T&) {return std::extent<M>::value;}
    
    template <class C, class M>
    struct EnumGet
    {
      typedef typename MemberType<M>::T E;
      M m;
      EnumGet(M m): m(m) {}
      string operator()(const C& o) const {return enum_keys<E>()(o.*m);}
    };
  
    template <class C,class M>
    struct EnumSet
    {
      typedef typename MemberType<M>::T E;
      M m;
      EnumSet(M m): m(m) {}
      void operator()(C& o, const string& v) const {o.*m=enum_keys<E>()(v);}
    };

    template <class C, class M>
    EnumGet<C,M> enumGet(M m) {return EnumGet<C,M>(m);}
    template <class C, class M>
    EnumSet<C,M> enumSet(M m) {return EnumSet<C,M>(m);}

    template <class T>
    typename T::value_type& getItemRef(T& c, size_t n)
    {
      if (n>=size_t(c.size()))
        throw std::out_of_range("index out of bounds");
      auto i=c.begin();
      std::advance(i,n);
      return *i;
    }

    // for a structured type, return a PythonRef
    template <class T>
    typename enable_if<Not<PythonBasicType<typename T::value_type> >,
                       //PythonRef<typename T::value_type> >::T
                       typename T::value_type& >::T
    getItem(T& c, size_t n) {return getItemRef(c,n);}

    
    // for a basic python type, return by value
    template <class T>
    typename enable_if<PythonBasicType<typename T::value_type>,
                         typename T::value_type>::T
    getItem(T& c, size_t n) {return getItemRef(c,n);}

    template <class T>
    typename enable_if<Not<PythonBasicType<typename T::value_type> >,
                         void>::T
    setItem(T&, size_t,const typename T::value_type&) {}

    template <class T>
    typename enable_if<PythonBasicType<typename T::value_type>,
                         void>::T
    setItem(T& c, size_t n,const typename T::value_type& v)
    {getItemRef(c,n)=v;}

    template <class T>
    typename T::mapped_type basicGetMapItem(T& x, const typename T::key_type& k)
    {
      auto i=x.find(k);
      if (i!=x.end())
        return i->second;
      else
        throw std::runtime_error("key not found");
    }
      
    template <class T>
    typename enable_if<PythonBasicType<typename T::mapped_type>,
                       typename T::mapped_type>::T
    getMapItem(T& x, const typename T::key_type& k)
    {return basicGetMapItem(x,k);}
    
    template <class T>
    typename enable_if<Not<PythonBasicType<typename T::mapped_type> >,
                       //PythonRef<typename T::mapped_type> >::T
                       typename T::mapped_type& >::T
    getMapItem(T& x, const typename T::key_type& k)
    {return basicGetMapItem(x,k);}
      
    template <class T>
    void setMapItem
    (T& x, const typename T::key_type& k, const typename T::mapped_type& v)
    {x[k]=v;}

    /// exception to signal end of iteration
    struct StopIteration {};
    
    template <class T>
    struct Iterator
    {
      typedef typename T::const_iterator I;
      I i, end;
      Iterator() {}
      Iterator(I begin, I end): i(begin), end(end) {}
      typename T::key_type next() {
        if (i==end)
          throw StopIteration();
        else
          {
            auto j=i++;
            return j->first;
          }
      }
      static void registerClass(python_t&);
    };

    template <class T>
    Iterator<T> iter(const T& m) {return Iterator<T>(m.begin(), m.end());}


    // a one shot switch that determines if a give type has already been defined
    template <class T>
    inline bool classDefStarted()
    {
      static bool value=false;
      if (value) return true;
      value=true;
      return false;
    }
      

  }

  template <class T, int rank> struct tn<pythonDetail::ArrayGet<T,rank>>
  {
    static std::string name()
    {return "pythonDetail::ArrayGet<"+typeName<T>()+","+std::to_string(rank)+">";}
  };

  template <class T>
  struct tn<pythonDetail::Iterator<T>>
  {
    static string name() {return "classdesc::pythonDetail::Iterator<"+typeName<T>()+">";}
  };

  
}


// extend boost::python function signature processing on callable objects
namespace boost {
  namespace python {
    namespace detail {
      template <class F, class T>
      typename classdesc::pythonDetail::Sig<F>::T get_signature(F)
      {return typename classdesc::pythonDetail::Sig<F>::T();}
      template <class F, class T>
      typename classdesc::pythonDetail::Sig<F>::T get_signature(F,T*)
      {return typename classdesc::pythonDetail::Sig<F>::T();}
    }
  }
}

#include "boost/python.hpp"
#include <Python.h>
#include <vector>

namespace classdesc
{
  template <class T>
  typename enable_if<Not<PythonBasicType<T>>, boost::python::return_internal_reference<>>::T
  return_policy() {return boost::python::return_internal_reference<>();}
    
  template <class T>
  typename enable_if<PythonBasicType<T>, boost::python::default_call_policies>::T
  return_policy() {return boost::python::default_call_policies();}

  namespace pythonDetail
  {
    template <class F, int arity=Arity<F>::value==2>
    struct is_rawFunction;

    // used for detecting a raw function R f(tuple, dict) that can be
    // used to implement variable and named argument functions,
    // including implementing overload dispatching
    template <class F> struct is_rawFunction<F,true>
    {
      // strips reference and const qualifiers for the ith argument
      template <int i> struct Arg
      {
        typedef typename remove_const<typename remove_reference<typename functional::Arg<F,i>::T>::type>::type T;
      };
      
      static const bool value=
        is_same<typename Arg<1>::T,boost::python::tuple>::value &&
        is_same<typename Arg<2>::T,boost::python::dict>::value;
    };
    
    template <class F> struct is_rawFunction<F,false>
    {
      static const bool value=false;
    };

    template <class T>
    boost::python::object rawInit(const boost::python::tuple& args, const boost::python::dict&) {
      if (boost::python::len(args)>2)
        {
          auto& x=boost::python::extract<T&>(args[0])();
          auto el=boost::python::extract<boost::python::list>(args[1]);
          if (el.check())
            assignList(x,el);
        }
      return boost::python::object(); //ie None
    }
    
    // *** enum conversion to/from python string ***
#if PY_MAJOR_VERSION < 3
    inline const char* to_string(PyObject* x) {return PyString_AsString(x);}
#else
    inline const char* to_string(PyObject* x) {return PyUnicode_AsUTF8(x);}
#endif    
    template <class E>
    struct EnumToStr
    {
      static PyObject* convert(E e)
      {
        return boost::python::incref
          (boost::python::object
           (classdesc::enum_keys<E>()(e)).ptr());
      }
    };

    template <class E>
    struct EnumFromStr
    {
      EnumFromStr()
      {
        boost::python::converter::registry::push_back
          (&convertible, &construct,
           boost::python::type_id<E>());
      }

      static void* convertible(PyObject* obj_ptr)
      {
        // Extract the character data from the python string
        const char* value = to_string(obj_ptr);
        if (!value || !classdesc::enum_keys<E>().has(value)) return 0;
        return obj_ptr;
      }
 
      static void construct(
                            PyObject* obj_ptr,
                            boost::python::converter::rvalue_from_python_stage1_data* data)
      {
        // Extract the character data from the python string
        const char* value = to_string(obj_ptr);
 
        // Verify that obj_ptr is a string (should be ensured by convertible())
        assert(value);
 
        // Grab pointer to memory into which to construct the new QString
        void* storage = (
                         (boost::python::converter::rvalue_from_python_storage<E>*)
                         data)->storage.bytes;
 
        // in-place construct the new QString using the character data
        // extraced from the python object
        new (storage) E(classdesc::enum_keys<E>()(value));
        
        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
      }

    };

    //*******

//    template <class T>
//    struct PyInitWrap: public boost::python::def_visitor<PyInitWrap<T>>
//    {
//      template <class U>
//      void visit(U& c) const {
//        c.def("__init__",&assignList<T>);
//      }
//    };


  }

  class python_t
  {
    struct Scope
    {
      struct PythonDummy {};
      string name;
      boost::python::class_<PythonDummy> object;
      shared_ptr<boost::python::scope> scope;
      Scope(const string& name):
        name(name), object(name.c_str()), scope(new boost::python::scope(object)) {}
      // default object refers to current module
      Scope(): name(PyModule_GetName(boost::python::scope().ptr())),
               object(name.c_str()), scope(new boost::python::scope(object)) {}
    };
    std::vector<Scope> scopeStack; //push back current module onto stack
    boost::python::scope topScope;
    
  public:

    /// @{
    /// class registry
    struct ClassBase
    {
      virtual ~ClassBase() {}
      bool completed=false, started=false;

      template <class T, bool copiable> struct PyClass;
      template <class T> struct PyClass<T,true>: public boost::python::class_<T>
      {
        PyClass(const char* n): boost::python::class_<T>(n){}
      };
      template <class T> struct PyClass<T,false>: public boost::python::class_<T,boost::noncopyable>
      {
        PyClass(const char* n): boost::python::class_<T,boost::noncopyable>(n){}
      };
      
    };
    
    template <class T, bool copiable> struct Class:
     public ClassBase, public ClassBase::PyClass<T,copiable>
    {
      Class(const string& name): ClassBase::PyClass<T,copiable>(name.c_str()) {}
      
      // distinguish between assignable and unassignable properties, which may be const, or may have assignment deleted
      template <class X>
      typename enable_if<
        And<
        std::is_copy_assignable<typename pythonDetail::MemberType<X>::T>,
          Not<is_const<typename pythonDetail::MemberType<X>::T>>
          >,void>::T
      addProperty(const string& d, X x) {this->def_readwrite(d.c_str(),x);}
      
      template <class X>
      typename enable_if<
        Or<
          Not<std::is_copy_assignable<typename pythonDetail::MemberType<X>::T>>,
          is_const<typename pythonDetail::MemberType<X>::T>
          >,void>::T
      addProperty(const string& d, X x) {this->def_readonly(d.c_str(),x);}
      
      using PyClass<T,copiable>::def;
      // handle "raw" functions, enabling variable arguments and overload dispatch
      template <class F>
      typename enable_if<pythonDetail::is_rawFunction<F>, Class&>::T
      def(const char* n, F f) {
        static_assert(Not<is_reference<typename functional::Return<F>::T>>::value,
                      "\nreference return of raw function not supported.\nUse boost::python::ptr instead");
        PyClass<T,copiable>::def(n,boost::python::raw_function(f));
        return *this;
      }
      template <class F>
      typename enable_if<Not<pythonDetail::is_rawFunction<F>>, Class&>::T
      def(const char* n, F f) {PyClass<T,copiable>::def(n,f); return *this;}

      template <class R, class... Args>
      typename enable_if<is_reference<R>, Class&>::T
      overload(const char* n, R (T::*m)(Args...)) {
        PyClass<T,copiable>::def(n,m,boost::python::return_internal_reference<>());
        return *this;
      }
      template <class R, class... Args>
      typename enable_if<Not<is_reference<R>>, Class&>::T
      overload(const char* n, R (T::*m)(Args...)) {
        PyClass<T,copiable>::def(n,m);
        return *this;
      }
      template <class R, class O,class... Args>
      typename enable_if<is_reference<R>, Class&>::T
      overload(const char* n, R (T::*m)(Args...),const O& o) {
        PyClass<T,copiable>::def(n,m,o[boost::python::return_internal_reference<>()]);
        return *this;
      }
      template <class R, class O, class... Args>
      typename enable_if<Not<is_reference<R>>, Class&>::T
      overload(const char* n, R (T::*m)(Args...), const O& o) {
        PyClass<T,copiable>::def(n,m,o);
        return *this;
      }
      
      template <class R, class... Args>
      typename enable_if<is_reference<R>, Class&>::T
      overload(const char* n, R (T::*m)(Args...) const) {
        PyClass<T,copiable>::def(n,m,boost::python::return_internal_reference<>());
        return *this;
      }
      template <class R, class... Args>
      typename enable_if<Not<is_reference<R>>, Class&>::T
      overload(const char* n, R (T::*m)(Args...) const) {
        PyClass<T,copiable>::def(n,m);
        return *this;
      }
      template <class R, class O,class... Args>
      typename enable_if<is_reference<R>, Class&>::T
      overload(const char* n, R (T::*m)(Args...) const,const O& o) {
        PyClass<T,copiable>::def(n,m,o[boost::python::return_internal_reference<>()]);
        return *this;
      }
      template <class R, class O, class... Args>
      typename enable_if<Not<is_reference<R>>, Class&>::T
      overload(const char* n, R (T::*m)(Args...) const, const O& o) {
        PyClass<T,copiable>::def(n,m,o);
        return *this;
      }

    };
    
    // lazy instantiation pattern to avoid needing an object file, and
    // link time ordering dependencyy

    //can be cleared after all python descriptor processing has
    // finished to free memory, otherwise the descriptor will
    // potentially throw an exception if this vector is cleared
    static std::vector<shared_ptr<ClassBase> >& classes()
    {
      static std::vector<shared_ptr<ClassBase> > impl;
      return impl;
    }
  
    // lazy instantiation pattern to register a unique class object
    // per type.
    template <class T>
    Class<T,is_copy_constructible<T>::value>& getClass()
    {
      typedef Class<T,is_copy_constructible<T>::value> C;
      static size_t id=classes().size();
      if (id==classes().size())
        {
          // for now, put everything in global scope
          boost::python::scope scope(topScope);
          classes().push_back(shared_ptr<ClassBase>
                              (new C(typeName<T>())));
        }
      else if (id>classes().size())
        throw exception("classes registry no longer valid");
      return dynamic_cast<C&>(*classes()[id]);
    }
    /// @}
  
    string tail(const string& d) {
      size_t p=d.rfind('.');
      if (p==string::npos)
        return d;
      else
        return d.substr(p+1);
    }
    
    void checkScope(string d) {
      size_t level=0;
      for (size_t p=d.find('.'); p!=string::npos; p=d.find('.'), level++)
        {
          string head=d.substr(0,p);
          if (level<scopeStack.size() && head!=scopeStack[level].name)
            scopeStack.erase(scopeStack.begin()+level,scopeStack.end());
          if (level==scopeStack.size())
            scopeStack.push_back(Scope(head));
          d=d.substr(p+1);
        }
      scopeStack.erase(scopeStack.begin()+level,scopeStack.end());
      // final component of d is not part of scope
    }

    // recursively define classes of arguments
    template <class F, int N>
    struct DefineArgClasses {
      static void define(python_t& p) {
        typedef typename remove_const<
          typename remove_reference<
            typename functional::Arg<F,N>::T
            >::type
          >::type T;
        if (!pythonDetail::classDefStarted<T>())
          p.defineClass<T>();
        DefineArgClasses<F,N-1>::define(p);
      }
    };

    template <class F>
    struct DefineArgClasses<F,0> {
      static void define(python_t& p) {
        typedef typename remove_const<
          typename remove_reference<
            typename functional::Return<F>::T
            >::type
          >::type T;
        if (!pythonDetail::classDefStarted<T>())
          // define return type
          p.defineClass<T>();
      }
    };
    
    template <class T>
    typename enable_if<std::is_copy_assignable<T>,void>::T
    addObject(const string& d, T& o) {
      using namespace boost::python;
      checkScope(d);
      
      if (!scopeStack.empty())
        scopeStack.back().object.def_readwrite(tail(d).c_str(),o);
      else
        extract<dict>(scope().attr("__dict__"))()[tail(d).c_str()]=ptr(&o);
    }
    template <class T>
    typename enable_if<Not<std::is_copy_assignable<T>>,void>::T
    addObject(const string& d, const T& o) {
      using namespace boost::python;
      checkScope(d);
      if (!scopeStack.empty())
        scopeStack.back().object.def_readonly(tail(d).c_str(),o);
      else
        extract<dict>(scope().attr("__dict__"))()[tail(d).c_str()]=ptr(&o);
    }

    template <class F>
    typename enable_if<Not<is_pointer<typename functional::Return<F>::T>>, void>::T
    addFunctional(const string& d, F f) {
      checkScope(d);
      boost::python::def(tail(d).c_str(),f);
      if (!scopeStack.empty())
        scopeStack.back().object.staticmethod(tail(d).c_str());
      DefineArgClasses<F,functional::Arity<F>::value>::define(*this);
    }

    // ignore pointer returns, as we don't know anything about the object being pointed to.
    template <class F>
    typename enable_if<is_pointer<typename functional::Return<F>::T>, void>::T
    addFunctional(const string&, F) {}

    // no object present, update class definition
    template <class C, class M>
    typename enable_if<
      And<
        Not<is_reference<typename functional::Return<M>::T>>,
        Not<is_pointer<typename functional::Return<M>::T>>
        >,void>::T
    addMemberFunction(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
          c.def(tail(d).c_str(),m);
      DefineArgClasses<M,functional::Arity<M>::value>::define(*this);
    }
    
    // for methods returning a reference, create a wrapper object that
    // can be pythonified
    template <class C, class M>
    typename enable_if<is_reference<typename functional::Return<M>::T>,void>::T
    addMemberFunction(const string& d, M m) 
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m,boost::python::return_internal_reference<>());
      DefineArgClasses<M,functional::Arity<M>::value>::define(*this);
    }
    
    // ignore pointer returns, as we don't know anything about the object being pointed to.
    template <class C, class M>
    typename enable_if<is_pointer<typename functional::Return<M>::T>,void>::T
    addMemberFunction(const string&, M) {}

    template <class C, class M>
    typename enable_if<functional::is_nonmember_function_ptr<M>,void>::T
    addMemberObject(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m);
    }

    template <class C, class M>
    typename enable_if<And<Not<is_Carray<typename pythonDetail::MemberType<M>::T> >, Not<functional::is_nonmember_function_ptr<M> > >,void>::T
    addMemberObject(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.addProperty(tail(d),m);
      // ensure member type is registered
      DefineArgClasses<M,0>::define(*this);
    }
    
    template <class C, class M>
    typename enable_if<is_Carray<typename pythonDetail::MemberType<M>::T>,void>::T
    addMemberObject(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.add_property(tail(d).c_str(), pythonDetail::ArrayMemRef<C,M>(m),
                       pythonDetail::ArrayMemRef<C,M>(m));
      pythonDetail::ArrayMemRef<C,M>::L::registerClass(*this);
    }
    template <class C, class M>
    typename enable_if<is_member_function_pointer<M>,void>::T
    addMember(const string& d, M m) {addMemberFunction<C>(d,m);}
    
    template <class C, class M>
    typename enable_if<And<is_member_object_pointer<M>,
                           Not<functional::is_nonmember_function_ptr<M> > >,void>::T
    addMember(const string& d, M m) {addMemberObject<C>(d,m);}
    
    template <class C, class M>
    typename enable_if<
      And<
        functional::is_nonmember_function_ptr<M>,
        Not<is_pointer<typename functional::Return<M>::T>>
        >,void>::T
    addMember(const string& d, M m) {
      auto& c=getClass<C>();
      if (!c.completed)
          c.def(tail(d).c_str(),m);
    }
    
    // ignore pointer returns
    template <class C, class M>
    typename enable_if<
      And<
        functional::is_nonmember_function_ptr<M>,
        is_pointer<typename functional::Return<M>::T>
        >,void>::T
    addMember(const string&, M) {}

    template <class C, class T>
    void addStaticMember(const string& d, T* a) {
      auto& c=getClass<C>();
      if (!c.completed)
          c.addProperty(tail(d).c_str(),a);
    }

    template <class C, class M>
    void addEnum(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.add_property(tail(d).c_str(),pythonDetail::enumGet<C>(m),pythonDetail::enumSet<C>(m));
    }

    /// utility method to add a Python wrapper class for \a T
    template <class T>
    void defineClass() {python<T>(*this,"");}

  };

  /// convert a python list to a sequence type T
  template <class T>
  T convertListTo(const boost::python::list& y)
  {
    T x;
    for (int i=0; i<boost::python::len(y); ++i)
      x.push_back(boost::python::extract<typename T::value_type>(y[i]));
    return x;
  }
  
  template <class T>
  void assignList(T& x, const boost::python::list& y)
  {x=std::move(convertListTo<T>(y));}

  template <class T>
  boost::shared_ptr<T> constructFromList(const boost::python::list& y)
  {
    boost::shared_ptr<T> x(new T);
    assignList(*x,y);
    return x;
  }
//  template <class T>
//  T constructFromList(const boost::python::list& y)
//  {
//    T x;
//    assignList(x,y);
//    return x;
//  }
  
  template <class C, class T=C>
  typename enable_if<is_sequence<T>,void>::T
  python(python_t& p, const string& ) {
    auto& c=p.getClass<C>();
    if (!c.completed)
      c.def("__len__", &pythonDetail::len<T>).
        def("__getitem__", &pythonDetail::getItem<T>, return_policy<typename T::value_type>()).
        def("__setitem__", &pythonDetail::setItem<T>).
        def("assign",assignList<T>).
        def("__init__",boost::python::make_constructor(constructFromList<T>)).
        def("constructor",boost::python::make_constructor(constructFromList<T>));
    
    python<typename T::value_type>(p,"");
    
    //  python<typename pythonDetail::DePythonRef<typename functional::Return<decltype(&pythonDetail::getItem<T>)>::T>::T>(p,"");
      python<typename functional::Return<decltype(&pythonDetail::getItem<T>)>::T>(p,"");
  }
  
  template <class C, class T=C>
  typename enable_if<is_associative_container<T>,void>::T
  python(python_t& p, const string& ) {
    auto& c=p.getClass<C>();
    if (!c.completed)
      c.def("__len__", &pythonDetail::len<C>).
        def("__getitem__", &pythonDetail::getMapItem<C>).
        def("__setitem__", &pythonDetail::setMapItem<C>).
        def("__iter__", &pythonDetail::iter<C>);
    python<typename T::mapped_type>(p,"");
    python<typename T::key_type>(p,"");
    pythonDetail::Iterator<C>::registerClass(p);
  }

  template <class T>
  T& sharedPtrTargetGetter(const shared_ptr<T>& self)
  {
    if (self)
      return *self;
    else
      throw std::runtime_error("null dereference");
  }
  template <class T>
  typename enable_if<std::is_copy_assignable<T>, void>::T
  sharedPtrTargetSetter(const shared_ptr<T>& self,const T& v)
  {
    if (self)
      *self=v;
    else
      throw std::runtime_error("null dereference");
  }
  template <class T>
  typename enable_if<Not<std::is_copy_assignable<T>>, void>::T
  sharedPtrTargetSetter(const shared_ptr<T>& self,const T&)
  {
    if (self)
      throw std::runtime_error(typeName<T>()+" not assignable");
    else
      throw std::runtime_error("null dereference");
  }

  template <class T>
  typename enable_if<Not<is_enum<T>>,void>::T
  pythonSharedPtr(python_t& p, const string& d)
  {
    auto& c=p.getClass<classdesc::shared_ptr<T> >();
    if (!c.completed)
      c.add_property("target",
                     make_function(&sharedPtrTargetGetter<T>, return_policy<T>()),
                     &sharedPtrTargetSetter<T>);
    python<T>(p,d);
  }

  template <class T>
  string sharedPtrTargetEnumGetter(const shared_ptr<T>& self)
  {
    if (self)
      return enum_keys<T>()(*self);
    else
      throw std::runtime_error("null dereference");
  }
  template <class T>
  void sharedPtrTargetEnumSetter(const shared_ptr<T>& self,const string& v)
  {
    if (self)
      *self=enum_keys<T>()(v);
    else
      throw std::runtime_error("null dereference");
  }
  
  template <class T>
  typename enable_if<is_enum<T>,void>::T
  pythonSharedPtr(python_t& p, const string& d)
  {
    auto& c=p.getClass<classdesc::shared_ptr<T> >();
    if (!c.completed)
      c.add_property("target", &sharedPtrTargetEnumGetter<T>, &sharedPtrTargetSetter<T>);
    python<T>(p,d);
  }

  template <class F>
  typename enable_if<functional::is_nonmember_function_ptr<F>,void>::T
  python(python_t& p, const string& d, F f) {
    p.addFunctional(d,f);
  }

  template <class T>
  void python(python_t& p, const string& d, is_const_static, T* a)
  {
    p.addObject(tail(d),*a);
  }
  
  template <class C, class T, class M>
  void python_type(python_t& p, const string& d, is_const_static, M m)
  {
    p.addStaticMember<C>(d,m);
  }
  
  template <class C, class B, class M>
  typename enable_if<Not<is_enum<typename pythonDetail::MemberType<M>::T> >,void>::T
  python_type(python_t& p, const string& d, M m)
  {
    p.addMember<C>(d,m);
  }

  template <class C, class B, class M>
  typename enable_if<is_enum<typename pythonDetail::MemberType<M>::T>,void>::T
  python_type(python_t& p, const string& d, M m) 
  {
    p.addEnum<C>(d,m);
  }
  
  template <class C, class T>
  void python_type(python_t& p, const string& d, Exclude<T> (C::*m))
  {
  }

  template <class T>
  void python_onbase(python_t& p, const string& d, T& a);

  namespace pythonDetail
  {
    
    inline void translate(StopIteration)
    {PyErr_SetString(PyExc_StopIteration,"");}
    static int dummy = (boost::python::register_exception_translator<StopIteration>(&translate),0);

    template <class T>
    void Iterator<T>::registerClass(python_t& p)
    {
      auto& c=p.getClass<Iterator<T>>();
      if (!c.completed)
        {
          c.def("__next__",&Iterator<T>::next);
          c.completed=true;
        }
    }
  }

  /// add a python reference to a C++ static object
  /// path is a '.' delimited name qualified by namespaces
  /// if no '.' present, object is inserted into global namespace

  /// Lifetime of \a object should exceed that of any reference to it
  /// in Python, so best used for logn lived objects. You can
  /// programmatically delete the python reference by call "del
  /// objectname" using boost::python::exec

  /// NB: this function has to be run after the requested module is
  /// created. That means in particular, it cannot be run in the
  /// initialiser of static variable, nor within the initialiser of
  /// the module in question. Please use python_t::addObject()
  /// instead. It can be used to insert an object into the global
  /// module, however within a module initialiser.
  template <class T>
  void addPythonObject(const std::string& path, T& object)
  {
    using namespace boost::python;
    auto lastDot=path.rfind('.');
    string module, name;
    if (lastDot==std::string::npos)
      {
        module="__main__";
        name=path;
      }
    else
      {
        module=path.substr(0,lastDot);
        name=path.substr(lastDot+1);
      }
    dict the_dict=extract<dict>(import(module.c_str()).attr("__dict__"));
    the_dict[name]=ptr(&object);
  }

  template <> inline void python<boost::python::object>(python_t&,const string&) {}
  template <> inline void python<boost::python::tuple>(python_t&,const string&) {}
  template <> inline void python<boost::python::list>(python_t&,const string&) {}
  template <> inline void python<boost::python::dict>(python_t&,const string&) {}
  template <> inline void python<boost::python::slice>(python_t&,const string&) {}
  template <> inline void python<boost::python::str>(python_t&,const string&) {}

  template <class E>
  typename enable_if<is_enum<E>, void>::T
  python(python_t&,const string&) {
    boost::python::to_python_converter<E,pythonDetail::EnumToStr<E> >();
    pythonDetail::EnumFromStr<E>();
  }
}

namespace classdesc_access
{
  namespace cd=classdesc;
  template <class T, class Enable=void> struct access_python;

  template <class T> struct access_python<cd::Exclude<T> >
  {
    template <class U>
    void type(cd::python_t&,const cd::string&) {}
  };

  template <class T> struct access_python<classdesc::shared_ptr<T> >
  {
    template <class U>
    void type(cd::python_t& p,const cd::string& d) {
      classdesc::pythonSharedPtr<T>(p,d);
    }
  };

}

using classdesc::python;
using classdesc::python_onbase;

#endif

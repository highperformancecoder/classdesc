/*
  @copyright Russell Standisgh 2018
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
    public And<And<is_class<T>, Not<is_container<T> > >, Not<is_associative_container<T> > > {};

  /// types that have a primitive representation in Python
  template <class T>
  struct PythonBasicType:
    public Or<is_fundamental<T>, is_string<T> > {};
  
  // objectless calls
  // classdesc generated
  template <class T, class Base=T>
  typename enable_if<ClassdescEnabledPythonType<Base>,void>::T
  python(python_t& p, const string& d);


  // anything that is not a class
  template <class T>
  typename enable_if<Not<is_class<T> >,void>::T
  python(python_t& p, const string& d) {}

  namespace pythonDetail
  {
    using namespace classdesc::functional;

    template <class M> struct MemberType {typedef M T;};
    template <class U, class V>
    struct MemberType<U (V::*)>
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
      static constexpr size_t rank=std::rank<M>::value;
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
        if (n>=c.size())
          throw std::out_of_range("index out of bounds");
        typename T::iterator i=c.begin();
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
    setItem(T& c, size_t n,const typename T::value_type& v) {}

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
      typename classdesc::pythonDetail::Sig<F>::T get_signature(F f,T*dummy=0)
      {return typename classdesc::pythonDetail::Sig<F>::T();}
    }
  }
}

#include "boost/python.hpp"
#include <vector>

namespace classdesc
{
  template <class T>
  typename enable_if<Not<PythonBasicType<T>>, boost::python::return_internal_reference<>>::T
  return_policy() {return boost::python::return_internal_reference<>();}
    
  template <class T>
  typename enable_if<PythonBasicType<T>, boost::python::default_call_policies>::T
  return_policy() {return boost::python::default_call_policies();}

    
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
      bool completed=false;
    };

    template <class T> struct Class:
      public ClassBase, public boost::python::class_<T>
    {
      Class(const string& name): boost::python::class_<T>(name.c_str()) {}
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
    Class<T>& getClass()
    {
      static size_t id=classes().size();
      if (id==classes().size())
        {
          // for now, put everything in global scope
          boost::python::scope scope(topScope);
          classes().push_back(shared_ptr<ClassBase>
                              (new Class<T>(typeName<T>())));
        }
      else if (id>classes().size())
        throw exception("classes registry no longer valid");
      return dynamic_cast<Class<T>&>(*classes()[id]);
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

    template <class T>
    void addObject(const string& d, T& o) {
      checkScope(d);
      
      if (!scopeStack.empty())
        scopeStack.back().object.def_readwrite(tail(d).c_str(),o);
    }
    template <class T>
    void addObject(const string& d, const T& o) {
      checkScope(d);
      scopeStack.back().object.def_readonly(tail(d).c_str(),o);
    }

    template <class F>
    void addFunctional(const string& d, F f) {
      checkScope(d);
      boost::python::def(tail(d).c_str(),f);
      scopeStack.back().object.staticmethod(tail(d).c_str());
    }

    // no object present, update class definition
    template <class C, class M>
    typename enable_if<Not<is_reference<typename functional::Return<M>::T>>,void>::T
    addMemberFunction(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
          c.def(tail(d).c_str(),m);
      python<typename functional::Return<M>::T>(*this,"");
    }
    
    // for methods returning a reference, create a wrapper object that
    // can be pythonified
    template <class C, class M>
    typename enable_if<is_reference<typename functional::Return<M>::T>,void>::T
    addMemberFunction(const string& d, M m) 
    {
      typedef typename std::remove_reference<C>::type CC; 
      auto& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m,boost::python::return_internal_reference<>());
      python<typename remove_reference<typename functional::Return<M>::T>::type>(*this,"");
    }

    template <class C, class M>
    typename enable_if<functional::is_nonmember_function_ptr<M>,void>::T
    addMemberObject(const string& d, M m)
    {
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m);
    }

    template <class C, class M>
    typename enable_if<And<Not<is_Carray<typename pythonDetail::MemberType<M>::T> >, Not<functional::is_nonmember_function_ptr<M> > >,void>::T
    addMemberObject(const string& d, M m)
    {
      // recursively register class pythonDetails for the member
      //python<typename pythonDetail::DePythonRef<typename functional::Return<M>::T>::T>(*this,d);
      python<typename functional::Return<M>::T>(*this,d);
      auto& c=getClass<C>();
      if (!c.completed)
        c.def_readwrite(tail(d).c_str(),m);
   }
    
    template <class C, class M>
    typename enable_if<is_Carray<typename pythonDetail::MemberType<M>::T>,void>::T
    addMemberObject(const string& d, M m)
    {
      static constexpr size_t rank=std::rank<typename pythonDetail::MemberType<M>::T>::value;
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
    typename enable_if<functional::is_nonmember_function_ptr<M>,void>::T
    addMember(const string& d, M m) {
      auto& c=getClass<C>();
      if (!c.completed)
          c.def(tail(d).c_str(),m);
    }

    template <class C, class M>
    void addEnum(const string& d, M m)
    {
      auto& c=getClass<C>();
      if (!c.completed)
        c.add_property(tail(d).c_str(),pythonDetail::enumGet<C>(m),pythonDetail::enumSet<C>(m));
    }
  };

  template <class C, class T=C>
  typename enable_if<is_sequence<T>,void>::T
  python(python_t& p, const string& ) {
    auto& c=p.getClass<C>();
    if (!c.completed)
      c.def("__len__", &pythonDetail::len<T>).
        def("__getitem__", &pythonDetail::getItem<T>, return_policy<typename T::value_type>()).
        def("__setitem__", &pythonDetail::setItem<T>);
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
  void sharedPtrTargetSetter(const shared_ptr<T>& self,const T& v)
  {
    if (self)
      *self=v;
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
    int dummy = (boost::python::register_exception_translator<StopIteration>(&translate),0);

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
}

namespace classdesc_access
{
  namespace cd=classdesc;
  template <class T> struct access_python;

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

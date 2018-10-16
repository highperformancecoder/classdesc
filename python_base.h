/*
  @copyright Russell Standish 2018
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
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
    public And<is_class<T>, Not<is_container<T> > > {};

  /// types that have a primitive representation in Python
  template <class T>
  struct PythonBasicType:
    public Or<is_fundamental<T>, is_string<T> > {};
  
  // objectless calls
  // classdesc generated
  template <class T, class Base=T>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  python(python_t& p, const string& d);


  // anything that is not a class
  template <class T>
  typename enable_if<Not<is_class<T> >,void>::T
  python(python_t& p, const string& d) {}

  namespace detail
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
    
    template <class F> struct Sig
    {
      typedef typename boost::mpl::push_front<
        typename SigArg<F,Arity<F>::V>::T,
        typename Return<F>::T
        >::type T;
    };
  
    // container support
    template <class T> struct Len
    {
      T& container;
      Len(T& container): container(container) {}
      size_t operator()(T&) const {
        return container.size();
      }
    };

    template <class T>
    size_t len(T& x) {return x.size();}
    
    template <class T> struct GetItem
    {
      T& container;
      GetItem(T& container): container(container) {}
      typename T::value_type operator()(T& c,size_t n) const {
        if (n>=container.size())
          throw std::out_of_range("index out of bounds");
        typename T::iterator i=container.begin();
        std::advance(i,n);
        return *i;
      }
    };

    template <class U> struct Sig<Len<U>>
    {
      typedef boost::mpl::vector<size_t,U&> T;
    };
    template <class U> struct Sig<GetItem<U>>
    {
      typedef boost::mpl::vector<typename U::value_type,U&,size_t> T;
    };
    template <class T>
    struct ArrayWrapper
    {
      typedef T value_type;
      T* data;
      size_t m_size;      
      ArrayWrapper(T* d=0, size_t s=0): data(d), m_size(s) {}
      size_t size() const {return m_size;}
      typedef T* iterator;
      typedef const T* const_iterator;
      iterator begin() {return data;}
      const_iterator begin() const {return data;}
      iterator end() {return data+m_size;}
      const_iterator end() const {return data+m_size;}
    };

    template <class T, int rank>
    struct Array {};
    

    template <class T, int rank>
    struct ArrayLen
    {
      size_t n;
      ArrayLen(size_t n=0): n(n) {}
      size_t operator()(Array<T,rank>&) {return n;}
    };

    template <class T, int rank>
    struct ArrayGet
    {
      T* x;
      size_t dims[rank];
      size_t stride;
      ArrayGet(T*x=0, const size_t* d=0):
        x(x), stride(1) {
        if (d)
          memcpy(&dims,d,sizeof(dims));
        for (size_t i=1; i<rank; ++i)
          stride*=dims[i];
      }
      ArrayGet<T,rank-1> getItem(size_t i) const {
        if (i<dims[0])
          {
            return ArrayGet<T,rank-1>(x+i*stride, dims+1);
          }
        throw std::out_of_range("index out of bounds");
      }
      ArrayGet<T,rank-1> operator()(Array<T,rank>&, size_t i) const 
        {return getItem(i);}
      size_t len() const {return dims[0];}
      
      // ensures this class is registered in the python type system
      static void registerClass(python_t&);
    };

    template <class T, class U>
    typename enable_if<is_assignable<T&,U>, void>::T
    assign(T& x, const U& y) {x=y;}

    template <class T, class U>
    typename enable_if<Not<is_assignable<T&,U> >, void>::T
    assign(T& x, const U& y) {throw std::runtime_error("assignment not supported between "+typeName<U>()+" and "+typeName<T>());}

    // registerClass is a MixIn because terminating rank differs from that of NewArrayGet
    template <class T, int rank>
    struct NewArrayGetRegisterClass
    {static void registerClass(python_t& p);};

    template <class T>
    struct NewArrayGetRegisterClass<T,0>
    {static void registerClass(python_t& p) {python<T>(p,"");}};

    template <class T, int rank> struct ArrayGetReturn;
    
    template <class T, int rank>
    struct NewArrayGet: public NewArrayGetRegisterClass<T,rank>
    {
      static_assert(rank==std::rank<T>::value);
      T* x;
      NewArrayGet(): x(0) {}
      NewArrayGet(T& x): x(&x) {}
      typedef typename ArrayGetReturn<T,rank>::T L; 
      L get(size_t i) const
      {return L((*x)[i]);}
      void set(size_t i, const L& v) {assign((*x)[i],v);}
    };

    template <class U, int rank> struct ArrayGetReturn
    {typedef NewArrayGet<typename std::remove_extent<U>::type,rank-1> T;};
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
      typedef NewArrayGet<MT,rank> L; 

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
    
    template <class U, class M>
    struct Sig<ArrayMemRef<U,M> >
    {
      typedef boost::mpl::vector<typename ArrayMemRef<U,M>::L,U&> T;
    };

    
    
    template <class U, class M>
    struct Sig<ArrayMemRefSetItem<U,M> >
    {
      typedef boost::mpl::vector<void,U&,size_t,typename ArrayMemRefSetItem<U,M>::V> T;
    };
   
    template <class T>
    struct ArrayGet<T,1>
    {
      T* x;
      size_t n;      
      ArrayGet(T*x=0, const size_t* d=0): x(x), n(*d) {}
      T getItem(size_t i) const {
        if (i<n)
          return x[i];
        throw std::out_of_range("index out of bounds");
      }
      void setItem(size_t i, const T& y) {assign(x[i],y);}
      T operator()(Array<T,1>&, size_t i) const {return getItem(i);}
      size_t len() const {return n;}
      // ensures this class is registered in the python type system
      static void registerClass(python_t&);
    };
    
    template <class U, int R> struct Sig<ArrayLen<U,R>>
    {
      typedef boost::mpl::vector<size_t,Array<U,R>&> T;
    };
    template <class U,int R> struct Sig<ArrayGet<U,R> >
    {
      typedef boost::mpl::vector<ArrayGet<U,R-1>,Array<U,R>&,size_t> T;
    };
    template <class U> struct Sig<ArrayGet<U,1> >
    {
      typedef boost::mpl::vector<U,Array<U,1>&,size_t> T;
    };

    template <class U> struct remove_ref{typedef U T;};
    template <class U> struct remove_ref<U&> {typedef U T;};
  
    template <class C>
    struct PythonRef
    {
      typename std::remove_reference<C>::type* o=nullptr;
      PythonRef() {}
      PythonRef(C& o): o(&o) {}
      struct NullException: public std::exception
      {
        NullException() {puts("throw null python reference");}
        const char* what() const noexcept override {
          return "null python reference";
        }
      };
      C& operator*() const {
        if (o)
          return *o;
        else
          throw NullException();
      }
    };
  
    /// returns the target type of a PythonRef, T otherwise
    template <class U>  struct DePythonRef {typedef U T;};
    template <class U>  struct DePythonRef<PythonRef<U> > {typedef U T;};
    
    template <class C, class M>
    struct MemFn
    {
      M m;
      MemFn(M m): m(m) {}
      template <class... A>
      typename Return<M>::T operator()(PythonRef<C>& o, A... a)
      {return ((*o).*m)(std::forward<A>(a)...);}
    };

    template <class C, class M>
    struct MemFnRef
    {
      M m;
      typedef PythonRef<typename Return<M>::T> R;
      MemFnRef(M m): m(m) {}
      template <class... A>
      R operator()(PythonRef<C>& o, A... a)
      {return R(((*o).*m)(std::forward<A>(a)...));}
    };

    template <class C,class M>
    struct Get
    {
      M m;
      Get(M m): m(m) {}
      typename MemberType<M>::T operator()(const PythonRef<C>& o)
      {return (*o).*m;}
    };
    template <class C, class M>
    struct Set
    {
      M m;
      Set(M m): m(m) {}
      void operator()(PythonRef<C>& o,const typename MemberType<M>::T& x)
      {(*o).*m=x;}
    };

    template <class C,class M>
    struct BoundRefMethod: public bound_method<C,M>
    {
      BoundRefMethod(C& o, M m): bound_method<C,M>(o,m) {}
      typedef PythonRef<typename remove_ref<typename functional::Return<M>::T>::T> R;
      template <class... Args>
      R operator()(Args... x) {
        return R(bound_method<C,M>::operator()(std::forward<Args>(x)...));
      }
    };

    template <class C, class M>
    struct Sig<BoundRefMethod<C,M>>
    {
      typedef typename boost::mpl::push_front<
        typename SigArg<M,Arity<M>::V>::T,
        PythonRef<typename remove_ref<typename Return<M>::T>::T>
        >::type T;
    };


    template <class C, class M>
    struct Sig<MemFn<C,M>>
    {
      typedef typename boost::mpl::push_front<
        typename boost::mpl::push_front<
        typename SigArg<M,Arity<M>::V>::T,
        PythonRef<C>&>::type,
        typename Return<M>::T
        >::type T;
    };
    template <class C, class M>
    struct Sig<MemFnRef<C,M>>
    {
      typedef typename boost::mpl::push_front<
        typename boost::mpl::push_front<
        typename SigArg<M,Arity<M>::V>::T,
        PythonRef<C>&>::type,
        PythonRef<typename Return<M>::T>
        >::type T;
    };
    template <class C, class M>
    struct Sig<Get<C,M>>
    {
      typedef typename boost::mpl::vector<typename MemberType<M>::T,const PythonRef<C>&>::type T;
    };
  
    template <class C, class M>
    struct Sig<Set<C,M>>
    {
      typedef typename boost::mpl::vector<void, PythonRef<C>&, const typename MemberType<M>::T&>::type T;
    };

    
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

    template <class C,class E> struct Sig<EnumGet<C,E> >
    {
      typedef typename boost::mpl::vector<string, const C&>::type T;
    };
    template <class C,class E> struct Sig<EnumSet<C,E> >
    {
      typedef typename boost::mpl::vector<void,C&,string>::type T;
    };

    template <class C, class M>
    EnumGet<C,M> enumGet(M m) {return EnumGet<C,M>(m);}
    template <class C, class M>
    EnumSet<C,M> enumSet(M m) {return EnumSet<C,M>(m);}

    template <class C, class M>
    struct EnumRefGet
    {
      typedef typename MemberType<M>::T E;
      M m;
      EnumRefGet(M m): m(m) {}
      string operator()(const PythonRef<C>& o) const {return enum_keys<E>()((*o).*m);}
    };
  
    template <class C,class M>
    struct EnumRefSet
    {
      typedef typename MemberType<M>::T E;
      M m;
      EnumRefSet(M m): m(m) {}
      void operator()(const PythonRef<C>& o, const string& v) const {(*o).*m=enum_keys<E>()(v);}
    };

    template <class C, class M>
    EnumRefGet<C,M> enumRefGet(M m) {return EnumRefGet<C,M>(m);}
    template <class C, class M>
    EnumRefSet<C,M> enumRefSet(M m) {return EnumRefSet<C,M>(m);}

    template <class C,class E> struct Sig<EnumRefGet<C,E> >
    {
      typedef typename boost::mpl::vector<string, const PythonRef<C>&>::type T;
    };
    template <class C,class E> struct Sig<EnumRefSet<C,E> >
    {
      typedef typename boost::mpl::vector<void,const PythonRef<C>&,string>::type T;
    };

//    template <class C, class E>
//    string enumGet(E (C::*m)) {return EnumGet<C,E>(m);}
//    template <class T>
//    void enumSet(Enum_handle<T>& a, const string& x) {a=x;}

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
                         PythonRef<typename T::value_type> >::T
    getItem(T& c, size_t n) {return getItemRef(c,n);}

    // for a basic python type, return by value
    template <class T>
    typename enable_if<PythonBasicType<typename T::value_type>,
                         typename T::value_type>::T
    getItem(T& c, size_t n) {return getItemRef(c,n);}

    template <class T>
    typename functional::Return<decltype(&getItem<T>)>::T
    getItemPythonRef(PythonRef<T>& c, size_t n) {
      return getItem(*c,n);
    }
    
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
    void setItemPythonRef(PythonRef<T>& c, size_t n,const typename T::value_type& v)
    {setItem(*c,n,v);}
    
    template <class T>
    size_t lenPythonRef(detail::PythonRef<T>& x) {return (*x).size();}

  }

  template <class T> struct tn<detail::PythonRef<T>>
  {
    static std::string name()
    {return "PythonRef<"+typeName<T>()+">";}
  };

  template <class T, int rank> struct tn<detail::NewArrayGet<T,rank>>
  {
    static std::string name()
    {return "detail::NewArrayGet<"+typeName<T>()+","+std::to_string(rank)+">";}
  };


  
}


// extend boost::python function signature processing to bound member functions
namespace boost {
  namespace python {
    namespace detail {
      template <class F>
      typename classdesc::detail::Sig<F>::T get_signature(F f)
      {return typename classdesc::detail::Sig<F>::T();}
      template <class F, class T>
      typename classdesc::detail::Sig<F>::T get_signature(F f,T*dummy=0)
      {return typename classdesc::detail::Sig<F>::T();}
    }
  }
}

#include "boost/python.hpp"
#include <vector>

namespace classdesc
{
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
      auto& cr=getClass<detail::PythonRef<C>>();
      if (!cr.completed)
          cr.def(tail(d).c_str(),detail::MemFn<C,M>(m));
      python<typename detail::DePythonRef<typename functional::Return<M>::T>::T>(*this,"");
    }
    
    template <class C, class M>
    typename enable_if<Not<is_reference<typename functional::Return<M>::T>>,void>::T
    addMemberFunction(const string& d, C& o, M m) {
      addFunctional(d,functional::bound_method<C,M>(o,m));
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m);
      addMemberFunction<C>(d,m);
    }
    // for methods returning a reference, create a wrapper object that
    // can be pythonified
    template <class C, class M>
    typename enable_if<is_reference<typename functional::Return<M>::T>,void>::T
    addMemberFunction(const string& d, M m) 
    {
      typedef typename std::remove_reference<C>::type CC; 
      auto& c=getClass<detail::PythonRef<CC>>();
      if (!c.completed)
        c.def(tail(d).c_str(),detail::MemFnRef<CC,M>(m));
      python<typename remove_reference<typename functional::Return<M>::T>::type>(*this,"");
    }

    template <class C, class M>
    typename enable_if<is_reference<typename functional::Return<M>::T>,void>::T
    addMemberFunction(const string& d, C& o, M m) {
      addFunctional(d,detail::BoundRefMethod<C,M>(o,m));
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),detail::BoundRefMethod<C,M>(o,m));
      addMemberFunction<C>(d,m);
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
    typename enable_if<functional::is_nonmember_function_ptr<M>,void>::T
      addMemberObject(const string& d, C& o, M m)
    {
      addFunctional(d,m);
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d),m);
      addMemberObject<C>(d,m);
    }

    template <class C, class M>
    typename enable_if<And<Not<is_Carray<typename detail::MemberType<M>::T> >, Not<functional::is_nonmember_function_ptr<M> > >,void>::T
    addMemberObject(const string& d, M m)
    {
      // recursively register class details for the member
      python<typename detail::DePythonRef<typename functional::Return<M>::T>::T>(*this,d);
      auto& c=getClass<C>();
      if (!c.completed)
        c.def_readwrite(tail(d).c_str(),m);
      auto& cr=getClass<detail::PythonRef<C>>();
      if (!cr.completed)
        cr.add_property(tail(d).c_str(),detail::Get<C,M>(m),
                       detail::Set<C,M>(m));
    }

    template <class C, class M>
    typename enable_if<is_Carray<typename detail::MemberType<M>::T>,void>::T
    addMemberObject(const string& d, M m)
    {
      static constexpr size_t rank=std::rank<typename detail::MemberType<M>::T>::value;
      auto& c=getClass<C>();
      if (!c.completed)
        c.add_property(tail(d).c_str(), detail::ArrayMemRef<C,M>(m),
                       detail::ArrayMemRef<C,M>(m));
      detail::ArrayMemRef<C,M>::L::registerClass(*this);
    }

   template <class C, class M>
    typename enable_if<Not<functional::is_nonmember_function_ptr<M> >,void>::T
    addMemberObject(const string& d, C&, M m) {
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def_readwrite(tail(d).c_str(),m);
      addMemberObject<C>(d,m);
    }
    template <class C, class M>
    void addMemberObject(const string& d, const C&, M m) {
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def_readonly(tail(d).c_str(),m);
      {
        auto c=getClass<detail::PythonRef<C>>();
        if (!c.completed)
          c.add_property(tail(d).c_str(),detail::Get<C,M>(m));
      }
    }
    template <class C, class M>
    typename enable_if<is_member_function_pointer<M>,void>::T
    addMember(const string& d, M m) {addMemberFunction<C>(d,m);}
    
    template <class C, class M>
    typename enable_if<is_member_object_pointer<M>,void>::T
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
        c.add_property(tail(d).c_str(),detail::enumGet<C>(m),detail::enumSet<C>(m));
      
      auto& cr=getClass<detail::PythonRef<C> >();
      if (!cr.completed)
        cr.add_property(tail(d).c_str(),detail::enumRefGet<C>(m),detail::enumRefSet<C>(m));
    }
  };

  template <class T>
  struct is_sequence<detail::ArrayWrapper<T> >: public true_type {};

  template <class T>
  typename enable_if<is_sequence<T>,void>::T
  python(python_t& p, const string& ) {
    auto& c=p.getClass<T>();
    if (!c.completed)
      c.def("__len__", &detail::len<T>).
      def("__getitem__", &detail::getItem<T>).
      def("__setitem__", &detail::setItem<T>);
    auto& cr=p.getClass<detail::PythonRef<T> >();
    if (!cr.completed)
      cr.def("__len__", &detail::lenPythonRef<T>).
        def("__getitem__", &detail::getItemPythonRef<T>).
        def("__setitem__", &detail::setItemPythonRef<T>);
    python<typename T::value_type>(p,"");
    
    python<typename detail::DePythonRef<typename functional::Return<decltype(&detail::getItem<T>)>::T>::T>(p,"");
  }

  template <class T>
  T& sharedPtrGetter(const shared_ptr<T>& self)
  {
    if (self)
      return *self;
    else
      throw std::runtime_error("null dereference");
  }
  
  template <class T>
  void python(python_t& p, const string& d, shared_ptr<T>& a)
  {//TODO
//    auto& c=p.getClass<T>();
//    if (!c.completed)
//      c.//def("get",&sharedPtrGetter).
//        def("__getitem__", &detail::getItem<shared_ptr<T> >).
//        def("__setitem__", &detail::setItem<shared_ptr<T> >);
//    //python<T>(p,"");
  }

  void python(python_t& p, const string& d, string& a) {
    p.addObject(d,a);
  }

  template <class F>
  typename enable_if<functional::is_nonmember_function_ptr<F>,void>::T
  python(python_t& p, const string& d, F f) {
    p.addFunctional(d,f);
  }

  template <class C, class B, class M>
  typename enable_if<Not<is_enum<typename detail::MemberType<M>::T> >,void>::T
  python_type(python_t& p, const string& d, M m)
  {
    p.addMember<C>(d,m);
  }

  template <class C, class B, class M>
  typename enable_if<is_enum<typename detail::MemberType<M>::T>,void>::T
  python_type(python_t& p, const string& d, M m) 
  {
    p.addEnum<C>(d,m);
  }
  
  template <class C, class T>
  void python_type(python_t& p, const string& d, Exclude<T> (C::*m))
  {
  }

  template <class T>
  void python_onbase(python_t& p, const string& d, T& a)
  {python(p,d,a);}
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
}

using classdesc::python;
using classdesc::python_onbase;

#endif

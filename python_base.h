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

  // objectless call
  template <class T>
  typename enable_if<is_class<T>,void>::T
  python(python_t& p, const string& d);

  template <class T>
  typename enable_if<Not<is_class<T> >,void>::T
  python(python_t& p, const string& d) {}

  namespace detail
  {
    using namespace classdesc::functional;

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
        typename Arg<F,Arity<F>::V-N+1>::T,
        typename SigArg<F,N-1>::T
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
    Len<T> len(T& x) {return Len<T>(x);}

    template <class T> struct GetItem
    {
      T& container;
      GetItem(T& container): container(container) {}
      typename T::value_type operator()(T&,size_t n) const {
        if (n>=container.size())
          throw std::out_of_range("index out of bounds");
        typename T::iterator i=container.begin();
        std::advance(i,n);
        return *i;
      }
    };

    template <class T>
    GetItem<T> getItem(T& x) {return GetItem<T>(x);}

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
      ArrayGet(T*x=0, const size_t* d=0): x(x) {
        memcpy(&dims,d,sizeof(dims));
      }
      ArrayGet<T,rank-1> getItem(size_t i) const {
        if (i<dims[rank-1])
          {
            size_t stride=1;
            for (size_t i=0; i<rank-1; ++i) stride*=dims[i];
            return ArrayGet<T,rank-1>(x+i*stride, dims);
          }
        throw std::out_of_range("index out of bounds");
      }
      ArrayGet<T,rank-1> operator()(Array<T,rank>&, size_t i) const 
        {return getItem(i);}
      size_t len() const {return dims[rank-1];}
      
      // ensures this class is registered in the python type system
      static void registerClass(python_t&);
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

    template <class M> struct MemberType;
    template <class U, class V>
    struct MemberType<U (V::*)>
    {
      typedef U T;
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
  }

  template <class T> struct tn<detail::PythonRef<T>>
  {
    static std::string name()
    {return "PythonRef<"+typeName<T>()+">";}
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
    };
    std::vector<Scope> scopeStack;
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
      auto& c=getClass<detail::PythonRef<C>>();
      if (!c.completed)
          c.def(tail(d).c_str(),detail::MemFn<C,M>(m));
      python<typename functional::Return<M>::T>(*this,"");
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
    typename enable_if<Not<functional::is_nonmember_function_ptr<M> >,void>::T
    addMemberObject(const string& d, M m)
    {
      auto& c=getClass<detail::PythonRef<C>>();
      if (!c.completed)
        c.add_property(tail(d).c_str(),detail::Get<C,M>(m),
                       detail::Set<C,M>(m));
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
  };

  template <class T>
  struct ClassdescEnabledPythonType:
    public Not<Or<is_fundamental<T>,is_container<T> > > {};

  template <class T>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  python(python_t& p, const string& d, T& a);

  template<class C, class M>
  typename enable_if<is_member_function_pointer<M>, void>::T
  python(python_t& p, const string& d, C& c, M m) {
    p.addMemberFunction(d,c,m);
  }
  
  template<class C, class M>
  typename enable_if<is_member_object_pointer<M>, void>::T
  python(python_t& p, const string& d, C& c, M m) {
    p.addMemberObject(d,c,m);
    python(p,d,c.*m);
  }
  
//  template<class C, class M>
//  typename enable_if<is_function<M>, void>::T
//  python(python_t& p, const string& d, C& c, M *m) {
//    p.addMemberFunctionPtr(d,c,m);
//  }
  
  template <class T>
  typename enable_if<is_fundamental<T>,void>::T
  python(python_t& p, const string& d, T& a) {
    p.addObject(d,a);
  }


  template <class T>
  struct is_sequence<detail::ArrayWrapper<T> >: public true_type {};

  template <class T>
  void python(python_t& p, const string& d, is_array,
                    T& arg, int dims, size_t dim1)
  {
    boost::python::class_<detail::Array<T,1>>((tail(d)+"_type").c_str()).
      def("__len__", detail::ArrayLen<T,1>(dim1)).
      def("__getitem__", detail::ArrayGet<T,1>(&arg,&dim1));
    p.addObject(d,reinterpret_cast<detail::Array<T,1>&>(arg));
    detail::ArrayGet<T,1>::registerClass(p);
  }

  template <class T>
  void python(python_t& p, const string& d, is_array,
                    T& arg, int, size_t dim1, size_t dim2)
  {
    size_t dims[]={dim1,dim2};
    boost::python::class_<detail::Array<T,2>>((tail(d)+"_type").c_str()).
      def("__len__", detail::ArrayLen<T,2>(dim2)).
      def("__getitem__", detail::ArrayGet<T,2>(&arg,dims));
    p.addObject(d,reinterpret_cast<detail::Array<T,2>&>(arg));
    detail::ArrayGet<T,2>::registerClass(p);
  }

  
  template <class T>
  typename enable_if<is_sequence<T>,void>::T
  python(python_t& p, const string& d, T& a) {
    boost::python::class_<T>((tail(d)+"_type").c_str()).
      def("__len__", detail::len(a)).
      def("__getitem__", detail::getItem(a));
    p.addObject(d,a);
  }

  template <class T>
  typename enable_if<is_associative_container<T>,void>::T
  python(python_t& p, const string& d, T& a) {
    boost::python::class_<T>((tail(d)+"_type").c_str()).
      def("__iter__", boost::python::iterator<T>()).
      def("__len__", functional::bindMethod(a,&T::size));
    p.addObject(d,a);
  }

  template <class T>
  void python(python_t& p, const string& d, const Enum_handle<T>& a) {
    p.addObject(d,a);
  }
  
  template <class T>
  void python(python_t& p, const string& d, Exclude<T>& a) {}
 
  template <class T>
  void python(python_t& p, const string& d, shared_ptr<T>& a)
  {//TODO
  }

  void python(python_t& p, const string& d, string& a) {
    p.addObject(d,a);
  }

  template <> void python<string>(python_t& p, const string& d) {}
  
  template <class M>
  void python_type(python_t& p, const string& d, M m)
  {
    p.addMember<typename functional::ClassOf<M>::T>(d,m);
  }

   
}

namespace classdesc_access
{
  namespace cd=classdesc;
  template <class T> struct access_python;
}

using classdesc::python;

#endif

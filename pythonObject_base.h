/*
  @copyright Russell Standish 2018
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef PYTHONOBJECT_BASE_H
#define PYTHONOBJECT_BASE_H
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
      size_t operator()() const {
        return container.size();
      }
    };

    template <class T>
    Len<T> len(T& x) {return Len<T>(x);}

    template <class T> struct GetItem
    {
      T& container;
      GetItem(T& container): container(container) {}
      typename T::value_type operator()(size_t n) const {
        if (n>=container.size())
          throw std::runtime_error("out of bounds");
        typename T::iterator i=container.begin();
        std::advance(i,n);
        return *i;
      }
    };

    template <class T>
    GetItem<T> getItem(T& x) {return GetItem<T>(x);}

    template <class U> struct Sig<Len<U>>
    {
      typedef boost::mpl::vector<size_t> T;
    };
    template <class U> struct Sig<GetItem<U>>
    {
      typedef boost::mpl::vector<typename U::value_type,size_t> T;
    };
  }
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

  class pythonObject_t
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

    template <class C, class M>
    void addMemberFunction(const string& d, C& o, M m) {
      addFunctional(d,functional::bound_method<C,M>(o,m));
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d).c_str(),m);
    }
    template <class C, class M>
    void addMemberFunctionPtr(const string& d, C& o, M *m) {
      addFunctional(d,m);
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def(tail(d),m);
    }
    template <class C, class M>
    void addMemberObject(const string& d, C& o, M m) {
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def_readwrite(tail(d).c_str(),m);
    }
    template <class C, class M>
    void addMemberObject(const string& d, const C& o, M m) {
      //addObject(d,o.*m);
      Class<C>& c=getClass<C>();
      if (!c.completed)
        c.def_readonly(tail(d).c_str(),m);
    }
  };

  template <class T>
  struct ClassdescEnabledPythonType:
    public Not<Or<is_fundamental<T>,is_container<T> > > {};

  template <class T>
  typename enable_if<ClassdescEnabledPythonType<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a);
  
  template<class C, class M>
  typename enable_if<is_member_function_pointer<M>, void>::T
  pythonObject(pythonObject_t& p, const string& d, C& c, M m) {
    p.addMemberFunction(d,c,m);
  }
  
  template<class C, class M>
  typename enable_if<is_member_object_pointer<M>, void>::T
  pythonObject(pythonObject_t& p, const string& d, C& c, M m) {
    p.addMemberObject(d,c,m);
    pythonObject(p,d,c.*m);
  }
  
  template<class C, class M>
  typename enable_if<is_function<M>, void>::T
  pythonObject(pythonObject_t& p, const string& d, C& c, M *m) {
    p.addMemberFunctionPtr(d,c,m);
  }
  
  template <class T>
  typename enable_if<is_fundamental<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a) {
    p.addObject(d,a);
  }

  template <class T>
  void pythonObject(pythonObject_t& p, const string& d, is_array,
                    T& arg, int dims, size_t ncopies,...)
  {
    //TODO
  }

  template <class T>
  typename enable_if<is_sequence<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a) {
    boost::python::class_<T>((tail(d)+"_type").c_str()).
      def("__iter__", boost::python::iterator<T>());
    p.addFunctional(d+".len", functional::bindMethod(a,&T::size));
    p.addFunctional(d+".get", detail::getItem(a));
    p.addObject(d,a);
  }

  template <class T>
  typename enable_if<is_associative_container<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a) {
    boost::python::class_<T>((tail(d)+"_type").c_str()).
      def("__iter__", boost::python::iterator<T>()).
      def("__len__", functional::bindMethod(a,&T::size));
    p.addObject(d,a);
  }

  template <class T>
  void pythonObject(pythonObject_t& p, const string& d, const Enum_handle<T>& a) {
    p.addObject(d,a);
  }
  
  template <class T>
  void pythonObject(pythonObject_t& p, const string& d, Exclude<T>& a) {}
 
  template <class T>
  void pythonObject(pythonObject_t& p, const string& d, shared_ptr<T>& a)
  {//TODO
  }

  void pythonObject(pythonObject_t& p, const string& d, string& a) {
    p.addObject(d,a);
  }


}

namespace classdesc_access
{
  template <class T> struct access_pythonObject;
}

using classdesc::pythonObject;

#endif

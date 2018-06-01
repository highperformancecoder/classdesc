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
#include "boost/python.hpp"
#include <vector>

namespace classdesc
{
  struct PythonDummy {};
  typedef boost::python::class_<PythonDummy> PDC;
  
  class pythonObject_t
  {
    struct Scope
    {
      string name;
      PDC object;
      shared_ptr<boost::python::scope> scope;
      Scope(const string& name): name(name), object(name.c_str()),
                                 scope(new boost::python::scope(object)) {}
    };
    std::vector<Scope> scopeStack;

  public:
    string tail(const string& d) {
      size_t p=d.rfind('.');
      if (p==string::npos)
        return d;
      else
        return d.substr(p+1);
    }
    
    void checkScope(string d) {
      for (size_t p=d.find('.'), level=0; p!=string::npos; p=d.find('.'), level++)
        {
          string head=d.substr(0,p);
          if (level<scopeStack.size() && head!=scopeStack[level].name)
            scopeStack.erase(scopeStack.begin()+level,scopeStack.end());
          if (level==scopeStack.size())
            scopeStack.push_back(Scope(head));
          d=d.substr(p+1);
        }
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
    void addFunction(const string& d, F f) {
      checkScope(d);
      scopeStack.back().object.def(f);
    }
      
    template <class C, class M>
    void addMemberFunction(const string& d, C& o, M m) {
      checkScope(d);
      scopeStack.back().object.def(functional::bound_method<C,M>(o,m));
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
  typename enable_if<is_container<T>,void>::T
  pythonObject(pythonObject_t& p, const string& d, T& a) {
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

#include "use_mbr_pointers.h"
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(pythonObject)
using classdesc::pythonObject;

#endif

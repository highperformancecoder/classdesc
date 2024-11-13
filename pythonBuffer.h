/*
  @copyright Steve Keen 2023
  @author Russell Standish
  This file is part of Minsky.

  Minsky is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Minsky is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Minsky.  If not, see <http://www.gnu.org/licenses/>.
*/

/* A buffer implementation for marshalling function arguments from python onjects
*/

#ifndef CLASSDESC_PYTHON_BUFFER_H
#define CLASSDESC_PYTHON_BUFFER_H

#include <json_pack_base.h>
#include <RESTProcess_base.h>
#include "signature.h"
#include <deque>
#include <numeric>
#include <Python.h>

#define CLASSDESC_PY_EXCEPTION_ABSORB(ret)                              \
  catch (const std::exception& ex)                                      \
    {                                                                   \
      PyErr_SetString(PyExc_RuntimeError, ex.what());                   \
      return ret;                                                       \
    }                                                                   \
  catch (...)                                                           \
    {                                                                   \
      PyErr_SetString(PyExc_RuntimeError, "Unknown exception");         \
      return ret;                                                   \
    }
  
namespace classdesc
{
  // map of registries, one per module
  inline std::map<std::string, RESTProcess_t*>& registries()
  {
    static std::map<std::string, RESTProcess_t*> registries;
    return registries;
  }

  namespace
  {
    // per compile unit registry
    RESTProcess_t registry;
    PyObject* pythonModule=nullptr;
  }
    
  /// @{ utility python object constructors
  inline PyObject* newPyObject(const bool& x) {if (x) Py_RETURN_TRUE; Py_RETURN_FALSE;}
  template <class T> inline typename enable_if<And<is_integral<T>, Not<is_same<T,bool>>>, PyObject*>::T
  newPyObject(const T& x) {return PyLong_FromLong(x);}
  template <class T> inline typename enable_if<is_floating_point<T>,PyObject*>::T
  newPyObject(const T& x) {return PyFloat_FromDouble(x);}
  inline PyObject* newPyObject(const string& x) {return PyUnicode_FromString(x.c_str());}
  inline PyObject* newPyObject(const char* x) {return PyUnicode_FromString(x);}

  // objects
  inline PyObject* newPyObjectJson(const json5_parser::mValue& j) {
    switch (j.type())
      {
      case json5_parser::obj_type:
        {
          auto pyObject=PyDict_New();
          for (auto& i: j.get_obj())
            PyDict_SetItemString(pyObject, i.first.c_str(), newPyObjectJson(i.second));
          return pyObject;
        }
      case json5_parser::array_type:
        {
          auto arr=j.get_array();
          auto pyObject=PyList_New(arr.size());
          for (size_t i=0; i<arr.size(); ++i)
            PyList_SetItem(pyObject, i, newPyObjectJson(arr[i]));
          return pyObject;
        }
      case json5_parser::str_type:
        return newPyObject(j.get_str());
      case json5_parser::bool_type:
        return newPyObject(j.get_bool());
      case json5_parser::int_type:
        return newPyObject(j.get_int64());
      case json5_parser::real_type:
        return newPyObject(j.get_real());
      case json5_parser::null_type:
        Py_RETURN_NONE;
      }
    assert(false); // silly compiler cannot figure out this is unreachable code
    Py_RETURN_NONE;
  }

  inline PyObject* newPyObject(const json_pack_t& j) {return newPyObjectJson(j);}

  /// sequences
  template <class T> inline typename enable_if<is_sequence<T>, PyObject*>::T
  newPyObject(const T& x) {
    auto pyObject=PyList_New(x.size());
    for (size_t i=0; i<x.size(); ++i)
      PyList_SetItem(pyObject, i, newPyObject(x[i]));
    return pyObject;
  }

  class PythonBuffer;
  inline PyObject* newPyObject(const PythonBuffer& x);

  template <class T> inline typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_sequence<T>>
      >, PyObject*>::T
  newPyObject(const T& x) {
    json_pack_t tmp; // go via JSON serialisation for arbitrary objects
    tmp<<x;
    return newPyObjectJson(tmp);
  }
  /// @}

  // container for handling PyObject lifetimes in an RAII fashion
  class PyObjectRef
  {
    PyObject* ref=nullptr;
  public:
    PyObjectRef()=default;
    PyObjectRef(PyObject* ref): ref(ref) {
      assert(!ref || Py_REFCNT(ref));
    }
    ~PyObjectRef() {Py_XDECREF(ref);}
    PyObjectRef(const PyObjectRef& x)=delete;
    PyObjectRef& operator=(const PyObjectRef& x)=delete;
    PyObjectRef(PyObjectRef&& x): ref(x.ref) {
      x.ref=nullptr;
      assert(!ref || Py_REFCNT(ref));
    }
    PyObjectRef& operator=(PyObjectRef&& x) {
      Py_XDECREF(ref);
      ref=x.ref;
      assert(!ref || Py_REFCNT(ref));
      x.ref=nullptr;
      return *this;
    }
    operator PyObject*() const {return ref;}
    operator bool() const {return ref;}
    PyObject* release() {auto tmp=ref; ref=nullptr; return tmp;}
  };
    
  class PythonBuffer
  {
  public:
    using Array=std::deque<PythonBuffer>;
    PythonBuffer() {Py_INCREF(pyObject);}
    
    explicit PythonBuffer(RESTProcessType::Type type) {
      switch (type)
        {
        case RESTProcessType::null: pyObject=Py_None; Py_INCREF(pyObject); return;
        case RESTProcessType::boolean: pyObject=Py_False; Py_INCREF(pyObject); return;
        case RESTProcessType::int_number: pyObject=PyLong_FromLong(0); return;
        case RESTProcessType::float_number: pyObject=PyFloat_FromDouble(0.0); return;
        case RESTProcessType::string: pyObject=PyUnicode_FromString(""); return;
        case RESTProcessType::object: pyObject=PyDict_New(); return;
        case RESTProcessType::array: pyObject=PyList_New(0); return;
        }
    }
    template <class T>
    explicit PythonBuffer(const T& x): pyObject(newPyObject(x)) {}
    explicit PythonBuffer(const json_pack_t& x): pyObject(newPyObjectJson(x)) {}
    explicit PythonBuffer(const json5_parser::mArray& x): pyObject(newPyObjectJson(json_pack_t(x))) {}
    explicit PythonBuffer(PyObject* x) {
      if (x) 
        pyObject=x; 
      else
        pyObject=Py_None;
      Py_XINCREF(x);
    }
    
    ~PythonBuffer() {Py_DECREF(pyObject);}

    PythonBuffer(const PythonBuffer& x): pyObject(x.pyObject) {Py_INCREF(pyObject);}
    PythonBuffer& operator=(const PythonBuffer& x)
    {pyObject=x.pyObject; Py_INCREF(pyObject); return *this;}
  
    /// return type conforms to the sequence concept
    Array array() const {
      Array r;
      if (PySequence_Check(pyObject))
        for (Py_ssize_t i=0; i<PySequence_Size(pyObject); ++i)
          r.push_back(PythonBuffer(PySequence_GetItem(pyObject,i)));
      else
        r.push_back(*this);
      return r;
    }
    RESTProcessType::Type type() const {
      if (pyObject==Py_None) return RESTProcessType::null;
      if (PyBool_Check(pyObject)) return RESTProcessType::boolean;
      if (PyLong_Check(pyObject)) return RESTProcessType::int_number;
      if (PyFloat_Check(pyObject)) return RESTProcessType::float_number;
      if (PyUnicode_Check(pyObject)) return RESTProcessType::string;
      if (PySequence_Check(pyObject)) return RESTProcessType::array;
      return RESTProcessType::object;
    }

    //template <class T> T get() const;
    template <class T> typename enable_if<is_integral<T>, T>::T
    get() const {return PyLong_AsLongLong(pyObject);}
    template <class T> typename enable_if<is_floating_point<T>,T>::T
    get() const {return PyFloat_AsDouble(pyObject);}
    //    template <> bool get<bool>() const {return PyLong_AsLong(pyObject);}
    template <class T> typename enable_if<is_string<T>, T>::T
    get() const {return PyUnicode_AsUTF8(pyObject);}
    
    template <class T> typename enable_if<is_same<T,json_pack_t>, T>::T
    get() const {
      switch (type())
        {
        case RESTProcessType::null:
          return json_pack_t(json5_parser::mValue::null);
        case RESTProcessType::boolean:
          return json_pack_t(get<bool>());
        case RESTProcessType::int_number:
          return json_pack_t(get<int64_t>());
        case RESTProcessType::float_number:
          return json_pack_t(get<double>());
        case RESTProcessType::string:
          return json_pack_t(get<string>());
        case RESTProcessType::array:
          {
            json_pack_t::Array arr;
            for (auto& i: array())
              arr.push_back(i.get<json_pack_t>());
            return json_pack_t(arr);
          }
        case RESTProcessType::object:
          {
            json_pack_t::Object obj;
            if (PyMapping_Check(pyObject))
              {
                PyObjectRef keyValues(PyMapping_Items(pyObject));
                for (Py_ssize_t i=0; i<PySequence_Size(keyValues); ++i)
                  {
                    PyObjectRef keyValue=PySequence_GetItem(keyValues, i);
                    PyObjectRef keyRef(PyObject_Str(PySequence_GetItem(keyValue,0)));
                    string keyStr=PyUnicode_AsUTF8(keyRef);
                    obj[keyStr]=PythonBuffer(PySequence_GetItem(keyValue,1)).get<json_pack_t>();
                  }
              }
            else
              {
                auto dir(PyObject_Dir(pyObject));
                for (Py_ssize_t i=0; i<PySequence_Size(dir); ++i)
                  {
                    auto key=PySequence_GetItem(pyObject, i);
                    PyObjectRef keyRef(PyObject_Str(key));
                    string keyStr=PyUnicode_AsUTF8(keyRef);
                    obj[keyStr]=PythonBuffer(PyObject_GetAttr(pyObject, key)).get<json_pack_t>();
                  }
              }
            return json_pack_t(obj);
          }
        }
      assert(false); // silly compiler cannot figure out this is unreachable code
      return json_pack_t();
    }
    
    template <class T> void push_back(const T& x) {
      if (PyList_Check(pyObject))
        {
          PythonBuffer pb(x);
          if (PyList_Append(pyObject, pb.pyObject))
            Py_INCREF(pb.pyObject); // TODO - does PySequence_SetItem already do this?
        }
    }

    /// returns a copy of the controlled object. Reference counter is incremented.
    PyObjectRef getPyObject() const {Py_INCREF(pyObject); return pyObject;}

    std::string str() const {
      PyObjectRef pyStr(PyObject_Str(pyObject));
      return PyUnicode_AsUTF8(pyStr);
    }
  private:
    PyObject* pyObject=Py_None; // note - this needs to be INCREF'd in constructors, not immortal before 3.12
    

  };

  inline PyObject* newPyObject(const PythonBuffer& x) {return x.getPyObject();}

  inline const PythonBuffer& operator>>(const PythonBuffer& b, char& x)
  {x=b.get<string>()[0]; return b;}

  inline PythonBuffer& operator<<(PythonBuffer& b, const char& x)
  {b=PythonBuffer(string{x}); return b;}

  // numbers
  template <class T>
  inline typename enable_if<
    And<is_arithmetic<T>,Not<is_const<T>>>,
    const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {x=b.get<T>(); return b;}

  template <class T>
  inline typename enable_if<
    is_arithmetic<T>, PythonBuffer&>::T
  operator<<(PythonBuffer& b, const T& x)
  {b=PythonBuffer(x); return b;}

  // strings
  template <class T>
  typename enable_if<And<is_string<T>,Not<is_const<T>>>, const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {x=b.get<string>(); return b;}

  template <class T>
  inline typename enable_if<And<is_string<T>>, PythonBuffer&>::T
  operator<<(PythonBuffer& b, const T& x)
  {b=PythonBuffer(string(x)); return b;}

  // enums
  template <class T>
  inline typename enable_if<And<is_enum<T>,Not<is_const<T>>>, const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {auto tmp=b.get<string>(); x=enum_keys<T>()(tmp); return b;}

  template <class T>
  inline typename enable_if<And<is_enum<T>>, PythonBuffer&>::T
  operator<<(PythonBuffer& b, const T& x)
  {b=PythonBuffer(to_string(x)); return b;}

  // sequences
  template <class T>
  inline typename enable_if<And<is_sequence<T>,Not<is_const<T>>>, const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {
    resize(x,0);
    if (b.type()!=RESTProcessType::array) return b;
    auto arr=b.array();
    for (auto& i: arr)
      {
        typename T::value_type e;
        i>>e;
        push_back(x,e);
      }
    return b;
  }

  template <class T>
  inline typename enable_if<is_sequence<T>, PythonBuffer&>::T
  operator<<(PythonBuffer& b, const T& x)
  {
    b=PythonBuffer(RESTProcessType::array);
    for (auto& i: x)
      {
        b.push_back(i);
      }
    return b;
  }

  // const
  template <class T>
  inline typename enable_if<is_const<T>, const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {return b;}


  inline const PythonBuffer& operator>>(const PythonBuffer& b, const char* x)
  {
    throw std::runtime_error("cannot unpack to char*, please use string instead");
  }
  
  // everything else
  template <class T>
  inline typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>,
      Not<is_enum<T>>,
      Not<is_const<T>>
      >, const PythonBuffer&>::T
  operator>>(const PythonBuffer& b, T& x)
  {
    b.get<json_pack_t>()>>x;
    return b;
  }

  template <class T>
  inline typename enable_if<
    And<
      Not<is_arithmetic<T>>,
      Not<is_string<T>>,
      Not<is_sequence<T>>,
      Not<is_enum<T>>
      >, PythonBuffer&>::T
  operator<<(PythonBuffer& b, const T& x)
  {
    json_pack_t tmp; tmp<<x;
    string s=write(tmp);
    b=PythonBuffer(s);
    return b;
  }

  /// C++ wrapper to default initialise the PyObject
  struct CppPyObject: public PyObject
  {
    CppPyObject() {memset(this,0,sizeof(PyObject));}
  };
  
  
  struct CppWrapper;
  void attachRegistryObjects(const RESTProcess_t& registry, CppWrapper& object,const std::string& prefix);

  struct CppWrapper: public CppPyObject
  {
    const bool special; // if true, command takes a key as an argument
    RPPtr command;
    std::map<string, PyObjectRef> methods;
    static CppWrapper* create(const RPPtr& command, bool special) {return new CppWrapper(command,special);}
    CppWrapper(CppWrapper&&)=default;

    static PyObject* returnResult(const RPPtr& result)
    {
      PythonBuffer resultBuffer(result->asBuffer());
      auto pyResult=resultBuffer.getPyObject();
      switch (resultBuffer.type())
        {
        case RESTProcessType::object:
        case RESTProcessType::array:
          {
            auto r=CppWrapper::create(result, false);
            PyObjectRef ref(r);
            attachRegistryObjects(result->list(),*r,".");
            return ref.release();
          }
        default: break;
        }
      if (PyErr_Occurred())
        PyErr_Print();
      return pyResult.release();
    }
    
    static PyObject* list(CppWrapper* self, PyObject*)
      try
        {
          auto methods=self->command->list();
          std::vector<string> methodList;
          for (auto& i: methods)
            if (!i.first.empty()) // don't include top level object
              methodList.push_back(i.first);
          return PythonBuffer(methodList).getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
      
    static PyObject* properties(CppWrapper* self, PyObject*)
      try
        {
          return PythonBuffer(self->command->asBuffer()).getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
      
    static PyObject* signature(CppWrapper* self, PyObject*)
      try
        {
          return PythonBuffer(self->command->signature()).getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
      
    static PyObject* type(CppWrapper* self, PyObject*)
      try
        {
          return PythonBuffer(self->command->type()).getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
     
    static PyObject* insert(CppWrapper* self, PyObject* value)
      try
        {
          self->command->insert(PythonBuffer(value).get<json_pack_t>());
          return Py_None;
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);

    static PyObject* erase(CppWrapper* self, PyObject* key)
      try
        {
          self->command->erase(PythonBuffer(key).get<json_pack_t>());
          return Py_None;
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);

    static PyObject* contains(CppWrapper* self, PyObject* key)
      try
        {
          return PythonBuffer(self->command->contains(PythonBuffer(key).get<json_pack_t>()))
            .getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);

    static PyObject* keys(CppWrapper* self, PyObject*)
      try
        {
          return PythonBuffer(self->command->keys()->asBuffer()).getPyObject().release();
        }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
      
  private:
    CppWrapper(const RPPtr& command,bool special); // private to force creation on heap
    CppWrapper(const CppWrapper&)=delete;
    void operator=(const CppWrapper&)=delete;
  };

  static PyMethodDef cppWrapperMethods[]={
    {"_list",(PyCFunction)CppWrapper::list,METH_NOARGS,"List of members/methods of this object"},
    {"_properties",(PyCFunction)CppWrapper::properties,METH_NOARGS,"Return current state of this object"},
    {"_signature",(PyCFunction)CppWrapper::signature,METH_NOARGS,"Signatures of all method overloads"},
    {"_type",(PyCFunction)CppWrapper::type,METH_NOARGS,"Object type"},
    {"insert",(PyCFunction)CppWrapper::insert,METH_O,"Insert element into container"},
    {"erase",(PyCFunction)CppWrapper::erase,METH_O,"Remove element from container"},
    {"keys",(PyCFunction)CppWrapper::keys,METH_NOARGS,"List keys of a map/set"},
    {"contains",(PyCFunction)CppWrapper::contains,METH_O,"Retrun true/false whether map/set contains a key"},
    {nullptr, nullptr, 0, nullptr}
  };

  struct CppWrapperType: public PyTypeObject
  {
    // container commands that take a key as as an argument
    static bool containerSpecialCommand(const std::string& command)
    {
      static const std::set<std::string> specialCommands{".@elem",".@elemNoThrow"};
      auto n=command.rfind('.');
      return n!=string::npos && specialCommands.count(command.substr(n));
    }
    
    static PyObject* call(PyObject* self, PyObject* args, PyObject *kwargs)
    {
      auto cppWrapper=static_cast<CppWrapper*>(self);
      auto command=cppWrapper->command;
      if (command->isObject() && PySequence_Size(args)==0)
        return CppWrapper::properties(cppWrapper,nullptr);
      PythonBuffer arguments(PySequence_Size(args)? RESTProcessType::array: RESTProcessType::null);
      std::string remainder;
      if (cppWrapper->special && PySequence_Size(args))
        // handle special commands which embed the argument in the path string
        remainder=write(PythonBuffer(PySequence_GetItem(args,0)).get<json_pack_t>());
      else
        for (Py_ssize_t i=0; i<PySequence_Size(args); ++i)
          arguments.push_back(PySequence_GetItem(args,i));
      if (PyErr_Occurred())
        PyErr_Print();
      try
        {
          return CppWrapper::returnResult(command->process(remainder,arguments.get<json_pack_t>()));
        }
      CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
    }

    static void deleteCppWrapper(PyObject* x) {
      delete static_cast<CppWrapper*>(x);
    }

    static PyObject* getAttro(PyObject* self, PyObject* attr)
      try
    {
      auto cppWrapper=static_cast<CppWrapper*>(self);
      auto i=cppWrapper->methods.find(PyUnicode_AsUTF8(attr));
      if (i!=cppWrapper->methods.end())
        {
          Py_INCREF(i->second);
          return i->second;
        }
      else
        {
          auto methods=cppWrapper->command->list();
          auto attribute=methods.find(string(".")+PyUnicode_AsUTF8(attr));
          if (attribute!=methods.end())
            return CppWrapper::create(attribute->second, false);
          }
      return PyObject_GenericGetAttr(self,attr);
    }
    CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
    
    static int setAttro(PyObject* self, PyObject* name, PyObject* attr)
    {
      auto cppWrapper=static_cast<CppWrapper*>(self);
      auto key=PyUnicode_AsUTF8(name);
      if (attr)
        {
          cppWrapper->methods[key]=PyObjectRef(attr);
          assert(Py_REFCNT(cppWrapper->methods[key]));
        }
      else
        cppWrapper->methods.erase(key);
      return 0;
    }

    struct MappingMethods: PyMappingMethods
    {
      static Py_ssize_t size(PyObject* self)
      {
        try
          {
            auto cppWrapper=static_cast<CppWrapper*>(self);
            return cppWrapper->command->size();
          }
        CLASSDESC_PY_EXCEPTION_ABSORB(0);
      }

      static PyObject* getElem(PyObject* self, PyObject* key)
      {
        auto cppWrapper=static_cast<CppWrapper*>(self);
        try
          {
            return CppWrapper::returnResult(cppWrapper->command->getElem(PythonBuffer(key).get<json_pack_t>()));
          }
        CLASSDESC_PY_EXCEPTION_ABSORB(nullptr);
      }
    
      static int setElem(PyObject* self, PyObject* key, PyObject* val)
      {
        auto cppWrapper=static_cast<CppWrapper*>(self);
        try
          {
            cppWrapper->command->setElem(PythonBuffer(key).get<json_pack_t>(),
                                         PythonBuffer(val).get<json_pack_t>());
            return 0;
          }
        catch (const std::exception& ex)
          {
            PyErr_SetString(PyExc_RuntimeError, ex.what());
            return -1;
          }
      }
      MappingMethods() {
        memset(this,0,sizeof(PyMappingMethods));
        mp_length=size;           // support for len
        mp_subscript=getElem;     // support for []
        mp_ass_subscript=setElem; // support for []=
      }
    } mappingMethods;

    CppWrapperType()
    {
      memset(this,0,sizeof(PyTypeObject));
      Py_INCREF(this);
      tp_name="CppWrapperType";
      tp_methods=cppWrapperMethods;
      tp_call=call;
      tp_basicsize=sizeof(CppWrapper);
      tp_dealloc=deleteCppWrapper;
      tp_getattro=getAttro;
      tp_setattro=setAttro;
      tp_as_mapping=&mappingMethods;
      PyType_Ready(this);
    }
  };

  inline CppWrapperType& CppWrapperTypeSingleton() {
    static CppWrapperType cppWrapperType;
    return cppWrapperType;
  }
  
  inline CppWrapper::CppWrapper(const RPPtr& command, bool special): special(special), command(command) {
    ob_refcnt=1;
    ob_type=&CppWrapperTypeSingleton();
  }

  inline void attachRegistryObjects(const RESTProcess_t& registry, CppWrapper& object,const std::string& prefix)
  {
    for (auto& i: registry)
      {
        std::string_view key(i.first);
        if (i.second && key.substr(0,prefix.length())==prefix)
          {
            std::string method(key.substr(prefix.length()));
            if (method.find('.')!=std::string::npos) continue;
            bool special=method[0]=='@';
            auto methodObject=CppWrapper::create(i.second, special);
            attachRegistryObjects(registry, *methodObject, i.first+".");
            if (special) method[0]='_';
            PyObject_SetAttrString(&object,method.c_str(),methodObject);
          }
      }
  }
    
  template <class F>
  struct RESTProcessFactory: public RESTProcessFunction<F>
  {
    RESTProcessFactory(F f): RESTProcessFunction<F>(f) {}
    RPPtr process(const string& remainder, const REST_PROCESS_BUFFER& arguments) override
    {
      JSONBuffer argBuf(arguments);
      // f is expected to return a unique_ptr
      return makeRESTProcessHeapObject
        (std::move(functional::callOnBuffer(argBuf,RESTProcessFunction<F>::f)));
    }
  };
  
  /// declare a factory object for a given type
  template <class T, class... Args> struct DeclareType
  {
    static std::unique_ptr<T> factory(Args... args)
    {return std::make_unique<T>(std::forward<Args>(args)...);}
    DeclareType(const string& typeName) {
      registry.add(typeName, new RESTProcessFactory<decltype(&factory)>(factory));
      //        registry.addFactory<T,Args...>(typeName,[](const std::string& name){
      //          auto object=std::make_shared<RESTProcessValueObject<T>>();
      //          auto cppObject=CppWrapper::create(object,false);
      //          PyObjectRef pyObject(cppObject);
      //          attachRegistryObjects(object->list(), *cppObject, ".");
      //          PyModule_AddObject(pythonModule, name.c_str(), pyObject.release());
      //        });
    }
  };

  inline void initModule(PyObject* pythonModule, RESTProcess_t& registry)
  {
    assert(pythonModule);
    for (auto& i: registry)
      { // loop over toplevel registry objects
        if (!i.second || i.first.empty() || i.first.find('.')!=std::string::npos || i.first[0]=='@') continue;
        auto pyObject=CppWrapper::create(i.second, false);
        attachRegistryObjects(registry, *pyObject, i.first+".");
        PyModule_AddObject(pythonModule, i.first.c_str(), pyObject);
      }
      
    // enum reflection
    PyObjectRef enummer=PyDict_New();
    auto enumList=registry.process("@enum.@list",{})->asBuffer();
    for (auto& i: enumList.array())
      {
        string name=i.get_str();
        PyDict_SetItemString(enummer, name.c_str(),
                             newPyObjectJson(registry.process("@enum."+name,{})->asBuffer()));
      }
    PyModule_AddObject(pythonModule, "enum", enummer.release());
  }

  inline PyObject* registerObject(PyObject* self, PyObject* args) {
    auto modName=PyModule_GetName(self);
    if (!modName || !registries().count(modName)) {
      PyErr_SetString(PyExc_RuntimeError, "Invalid module name");
      return nullptr;
    }
    if (PySequence_Size(args)<2) {
      PyErr_SetString(PyExc_RuntimeError, "Missing arguments");
      return nullptr;
    }
    auto object=PySequence_GetItem(args,0);
    if (!object || Py_TYPE(object)!=&CppWrapperTypeSingleton()) {
      PyErr_SetString(PyExc_RuntimeError, "First argument not a CppWrapper");
      return nullptr;
    }
    auto name=PyUnicode_AsUTF8(PySequence_GetItem(args,1));
    if (!name) {
      PyErr_SetString(PyExc_RuntimeError, "Second argument not a string");
      return nullptr;
    }
    (*registries()[modName])[name]=static_cast<CppWrapper*>(object)->command;
    if (PyModule_AddObject(self,name,object)==0)
      Py_INCREF(object);
    return Py_None;
  }
}


namespace classdesc_access
{
  template <>
  struct access_json_pack<classdesc::PythonBuffer> {
    template <class U>
    void operator()(classdesc::json_pack_t& j, const std::string&, U& x) {
      j<<x;
    }
  };
}

namespace {
  PyMethodDef defaultMethods[]={
    {"register",classdesc::registerObject,METH_VARARGS,"Register a C++ wrapped object with the registry"},
    {nullptr,nullptr,0,nullptr}
  };
}

/// a convenience macro for creating a python module with a single global object
/// @param name module name
/// @param object C++ object to expose to python
#define CLASSDESC_PYTHON_MODULE(name)                              \
  PyMODINIT_FUNC PyInit_##name()                                   \
  {                                                                \
    static PyModuleDef module_##name = {                           \
    PyModuleDef_HEAD_INIT,                                         \
    #name,                                                         \
    "Python interface to C++ code: "#name,                         \
    -1,                                                            \
    defaultMethods,                                                \
    nullptr,                                                       \
    nullptr,                                                       \
    nullptr,                                                       \
    nullptr                                                        \
    };                                                             \
                                                                   \
    using namespace classdesc;                                     \
    registries()[#name]=&registry;                                 \
    pythonModule=PyModule_Create(&module_##name);                  \
    if (pythonModule) initModule(pythonModule,registry);           \
    return pythonModule;                                           \
  }                                                 

/// Add a global object into the registry. \a object is also used as
/// the name this object will be referred to from Python, so must be
/// unqualified at the macro call.
#define CLASSDESC_ADD_GLOBAL(object)            \
  static int add_global_##object=(classdesc::RESTProcess(classdesc::registry,#object,object), 0);

#define CLASSDESC_ADD_FUNCTION(object)            \
  static int add_global_##object=(classdesc::RESTProcess(classdesc::registry,#object,&object), 0);


/// Add a type foundry or factory into the registry. \a type is also
/// used as the name this object will be referred to from Python, so
/// must be unqualified at the macro call. The following optional
/// arguments are a list of types of arguments passed to the
/// constructor, if any.
#define CLASSDESC_DECLARE_TYPE(type,...)                           \
  static classdesc::DeclareType<type __VA_OPT__(,) __VA_ARGS__> declareType_##type(#type);

#endif

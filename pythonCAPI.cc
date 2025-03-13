/*
  @copyright Russell Standish 2025
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// implements stubs to link to the Windows python dynamic library

#include "pythonCAPI.h"
#include <windows.h>
#include <iostream>

static HINSTANCE pythonExe=GetModuleHandle(nullptr);

extern "C"
{

#define APIFN(return,name,arg_decls,args)                               \
  return name arg_decls                                                 \
    {                                                                   \
    static auto symbol=(decltype(name)*)GetProcAddress(pythonExe, #name); \
    return symbol? symbol args: 0;                                      \
  }                                                                     \

#define VOID_APIFN(name,arg_decls,args)                                 \
  void name arg_decls                                                   \
  {                                                                     \
    static auto symbol=(decltype(name)*)GetProcAddress(pythonExe, #name); \
    if (symbol) symbol args;                                            \
  }                                                                     \

  VOID_APIFN(_Py_Dealloc,(PyObject*o),(o));
  APIFN(PyObject*, PyErr_Occurred, (), ());
  VOID_APIFN(PyErr_Print,()());
  VOID_APIFN(PyErr_SetString,(PyObject* o,const char* s),(o,s));
  
  APIFN(int, PyType_IsSubtype,(PyTypeObject* o1, PyTypeObject* o2),(o1,o2));
  APIFN(unsigned long, PyType_GetFlags,(PyTypeObject* o),(o));
  APIFN(PyObject*, PyLong_FromLong,(long x),(x));
  APIFN(PyObject*, PyFloat_FromDouble,(double x),(x));
  APIFN(long long, PyLong_AsLongLong, (PyObject* x), (x));
  APIFN(double, PyFloat_AsDouble, (PyObject* x), (x));
  APIFN(int, PyType_Ready, (PyTypeObject* t), (t));

  APIFN(PyObject*, PyObject_Str, (PyObject* o), (o));
  APIFN(PyObject*, PyObject_Dir, (PyObject* o), (o));
  APIFN(PyObject*, PyObject_GetAttr, (PyObject* o, PyObject* a), (o,a));
  APIFN(PyObject*, PyObject_GenericGetAttr, (PyObject* o, PyObject* a), (o,a));
  APIFN(int, PyObject_SetAttrString, (PyObject* o, const char* a, PyObject* v), (o,a,v));
  
  APIFN(int, PyModule_AddObject, (PyObject* o, const char* n, PyObject* v), (o,n,v));
  APIFN(const char*, PyModule_GetName, (PyObject* o), (o));
  APIFN(PyObject*, PyModule_Create2, (PyModuleDef* m,int i), (m,i));
  APIFN(int, PySequence_Check, (PyObject* o), (o));
  APIFN(ssize_t, PySequence_Size, (PyObject* o), (o));
  APIFN(PyObject*, PySequence_GetItem, (PyObject* o, ssize_t i), (o,i));
  
  APIFN(PyObject*, PyUnicode_FromString, (const char* s), (s));
  APIFN(char*, PyUnicode_AsUTF8, (PyObject* s), (s));

  APIFN(PyObject*, PyDict_New, (), ());
  APIFN(int, PyDict_SetItemString, (PyObject* d, const char* k, PyObject* v), (d,k,v));
  
  APIFN(PyObject*, PyList_New, (ssize_t size), (size));
  APIFN(int, PyList_SetItem, (PyObject* o, ssize_t i, PyObject* v), (o,i,v));
  APIFN(int, PyList_Append, (PyObject* o, PyObject* v), (o,v));
  
  APIFN(int, PyMapping_Check, (PyObject* o), (o));
  APIFN(PyObject*, PyMapping_Items, (PyObject* o), (o));
}

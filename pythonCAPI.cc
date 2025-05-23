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

static HINSTANCE pythonExe=GetModuleHandle("python3");

// On Windows, python3 refers to the stable ABI. checkSymbol provides
// an important check that all symbols referenced here are available
// in the stable ABI. This check should be performed prior to release.
static void checkSymbol(const char* name)
{
  auto symbol=GetProcAddress(pythonExe, name);
  if (!symbol)
    std::cerr<<name<<" not found in module"<<std::endl;
}
#define CHECK_SYMBOL(name) static int checkSymbol_##name=(checkSymbol(#name),1);

extern "C"
{

#define APIFN(returnType,name,arg_decls,args)                           \
  CHECK_SYMBOL(name)                                                    \
  returnType name arg_decls                                             \
  {                                                                     \
    if (!pythonExe) puts("python exe not found");                       \
    static auto symbol=(decltype(name)*)GetProcAddress(pythonExe, #name); \
    if (!symbol) puts("failed to load: "#name);                       \
    return symbol? symbol args: 0;                                        \
  }                                                                     

#define VOID_APIFN(name,arg_decls,args)                                 \
  CHECK_SYMBOL(name)                                                    \
  void name arg_decls                                                   \
  {                                                                     \
    static auto symbol=(decltype(name)*)GetProcAddress(pythonExe, #name); \
    if (!symbol) puts("failed to load: "#name);                         \
    if (symbol) symbol args;                                            \
  }                                                                     

#define APIVARPTR(type, name)                                 \
  type* name=(type*)GetProcAddress(pythonExe, #name);         \
  CHECK_SYMBOL(name)                                                 
  
  APIVARPTR(PyTypeObject, PyBool_Type);
  APIVARPTR(PyTypeObject, PyFloat_Type);
  APIVARPTR(PyObject, PyExc_RuntimeError);

  VOID_APIFN(_Py_Dealloc,(PyObject*o),(o));
  APIFN(PyObject*, PyErr_Occurred, (), ());
  VOID_APIFN(PyErr_Print,(),());
  VOID_APIFN(PyErr_SetString,(PyObject* o,const char* s),(o,s));
  APIFN(PyObject*, PyType_GenericAlloc,(PyTypeObject* t, Py_ssize_t n),(t,n));
  APIFN(PyObject*, PyType_GenericNew,(PyTypeObject* t, PyObject* a, PyObject* k),(t,a,k));

  APIFN(int,Py_IsInitialized,(),());
  APIFN(PyObject*,PySys_GetObject,(const char *name),(name));
  
  APIFN(PyObject*, Py_GetConstantBorrowed,(unsigned x),(x));
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
  APIFN(PyObject*, PyObject_Call, (PyObject* c, PyObject* a, PyObject* k),(c,a,k));

  APIFN(int, PyModule_AddObject, (PyObject* o, const char* n, PyObject* v), (o,n,v));
  APIFN(const char*, PyModule_GetName, (PyObject* o), (o));
  APIFN(PyObject*, PyModule_Create2, (PyModuleDef* m,int i), (m,i));
  APIFN(int, PySequence_Check, (PyObject* o), (o));
  APIFN(ssize_t, PySequence_Size, (PyObject* o), (o));
  APIFN(PyObject*, PySequence_GetItem, (PyObject* o, ssize_t i), (o,i));
  
  APIFN(PyObject*, PyUnicode_FromString, (const char* s), (s));
  APIFN(char*, PyUnicode_AsUTF8AndSize, (PyObject* s,Py_ssize_t* sz), (s,sz));

  APIFN(PyObject*, PyTuple_New, (Py_ssize_t s), (s));
  
  APIFN(PyObject*, PyDict_New, (), ());
  APIFN(int, PyDict_SetItemString, (PyObject* d, const char* k, PyObject* v), (d,k,v));
  
  APIFN(PyObject*, PyList_New, (ssize_t size), (size));
  APIFN(int, PyList_SetItem, (PyObject* o, ssize_t i, PyObject* v), (o,i,v));
  APIFN(int, PyList_Append, (PyObject* o, PyObject* v), (o,v));
  
  APIFN(int, PyMapping_Check, (PyObject* o), (o));
  APIFN(PyObject*, PyMapping_Items, (PyObject* o), (o));
}

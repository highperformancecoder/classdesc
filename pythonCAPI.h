/*
  @copyright Russell Standish 2025
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// Declares functions usded from the Python C API
#ifndef CLASSDESC_PYTHON_CAPI_H
#define CLASSDESC_PYTHON_CAPI_H
#include <cstddef>
#include <stdio.h>

// TODO - is this field used on Windows?
//#define _PyObject_HEAD_EXTRA

#define PYTHON_API_VERSION 1013

// Windows uses stable API, so the *Struct symbols are not available
#ifdef _WIN32
#define Py_False Py_GetConstantBorrowed(1)
#define Py_True Py_GetConstantBorrowed(2)
#define Py_None Py_GetConstantBorrowed(0)
#else
#define Py_False ((PyObject *) &_Py_FalseStruct)
#define Py_True ((PyObject *) &_Py_TrueStruct)
#define Py_None ((PyObject *) &_Py_NoneStruct)
#endif
#define Py_RETURN_TRUE return Py_INCREF(Py_True), Py_True
#define Py_RETURN_FALSE return Py_INCREF(Py_False), Py_False
#define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None

#define METH_VARARGS 0x0001
#define METH_NOARGS  0x0004
#define METH_O       0x0008

#define PyType_FastSubclass(t,f)  ((PyType_GetFlags(t) & (f)) != 0)

#define Py_TYPE(ob) (((PyObject*)(ob))->ob_type)
#define PyObject_TypeCheck(ob, tp) \
    (Py_TYPE(ob) == (tp) || PyType_IsSubtype(Py_TYPE(ob), (tp)))
#define PyBool_Check(x) (Py_TYPE(x) == PyBool_Type)
#define PyFloat_Check(op) PyObject_TypeCheck(op, PyFloat_Type)
#define PyLong_Check(op) PyType_FastSubclass(Py_TYPE(op), 1UL << 24)
#define PyList_Check(op) PyType_FastSubclass(Py_TYPE(op), 1UL << 25)
#define PyUnicode_Check(op) PyType_FastSubclass(Py_TYPE(op), 1UL << 28)

#define Py_REFCNT(ob) (((PyObject*)(ob))->ob_refcnt)
#define Py_INCREF(op) (((PyObject *)(op))->ob_refcnt++)
#define Py_DECREF(op)                                       \
  do {                                                      \
    PyObject *_py_decref_tmp = (PyObject *)(op);            \
    if (--(_py_decref_tmp)->ob_refcnt == 0)                 \
      _Py_Dealloc(_py_decref_tmp);                          \
  } while (0)

/* Macros to use in case the object pointer may be NULL: */
#define Py_XINCREF(op)                                \
    do {                                              \
        PyObject *_py_xincref_tmp = (PyObject *)(op); \
        if (_py_xincref_tmp != NULL)                  \
            Py_INCREF(_py_xincref_tmp);               \
    } while (0)

#define Py_XDECREF(op)                                \
    do {                                              \
        PyObject* _py_xdecref_tmp = (PyObject *)(op); \
        if (_py_xdecref_tmp != NULL)                  \
            Py_DECREF(_py_xdecref_tmp);               \
    } while (0)

#ifdef _WIN32
#define PyMODINIT_FUNC extern "C" __declspec(dllexport) PyObject*
#else
#define PyMODINIT_FUNC extern "C" PyObject*
#endif

#define PyObject_HEAD_INIT(type) {1, type },

#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }

#define PyModule_Create(module) \
        PyModule_Create2(module, PYTHON_API_VERSION)

// Windows in particular doesn't define ssize_t
using Py_ssize_t=std::ptrdiff_t;

struct PyTypeObject;

struct PyObject
{
  //_PyObject_HEAD_EXTRA
  Py_ssize_t ob_refcnt;
  PyTypeObject *ob_type;
};

struct PyVarObject
{
    PyObject ob_base;
    Py_ssize_t ob_size; /* Number of items in variable part */
};

typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef Py_ssize_t (*lenfunc)(PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);
typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef Py_ssize_t (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(struct _typeobject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(struct _typeobject *, Py_ssize_t);
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);
typedef int (*inquiry)(PyObject *);
typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);

struct PyAsyncMethods;
struct PyNumberMethods;
struct PySequenceMethods;
struct PyMappingMethods;
struct PyBufferProcs;
struct PyMethodDef;

struct  PyTypeObject
{
    PyVarObject ob_base;
    const char *tp_name; /* For printing, in format "<module>.<name>" */
    Py_ssize_t tp_basicsize, tp_itemsize; /* For allocation */

    /* Methods to implement standard operations */

    destructor tp_dealloc;
    printfunc tp_print;
    getattrfunc tp_getattr;
    setattrfunc tp_setattr;
    PyAsyncMethods *tp_as_async; /* formerly known as tp_compare (Python 2)
                                    or tp_reserved (Python 3) */
    reprfunc tp_repr;

    /* Method suites for standard classes */

    PyNumberMethods *tp_as_number;
    PySequenceMethods *tp_as_sequence;
    PyMappingMethods *tp_as_mapping;

    /* More standard operations (here for binary compatibility) */

    hashfunc tp_hash;
    ternaryfunc tp_call;
    reprfunc tp_str;
    getattrofunc tp_getattro;
    setattrofunc tp_setattro;

    /* Functions to access object as input/output buffer */
    PyBufferProcs *tp_as_buffer;

    /* Flags to define presence of optional/expanded features */
    unsigned long tp_flags;

    const char *tp_doc; /* Documentation string */

    /* Assigned meaning in release 2.0 */
    /* call function for all accessible objects */
    traverseproc tp_traverse;

    /* delete references to contained objects */
    inquiry tp_clear;

    /* Assigned meaning in release 2.1 */
    /* rich comparisons */
    richcmpfunc tp_richcompare;

    /* weak reference enabler */
    Py_ssize_t tp_weaklistoffset;

    /* Iterators */
    getiterfunc tp_iter;
    iternextfunc tp_iternext;

    /* Attribute descriptor and subclassing stuff */
    struct PyMethodDef *tp_methods;
    struct PyMemberDef *tp_members;
    struct PyGetSetDef *tp_getset;
    struct _typeobject *tp_base;
    PyObject *tp_dict;
    descrgetfunc tp_descr_get;
    descrsetfunc tp_descr_set;
    Py_ssize_t tp_dictoffset;
    initproc tp_init;
    allocfunc tp_alloc;
    newfunc tp_new;
    freefunc tp_free; /* Low-level free-memory routine */
    inquiry tp_is_gc; /* For PyObject_IS_GC */
    PyObject *tp_bases;
    PyObject *tp_mro; /* method resolution order */
    PyObject *tp_cache;
    PyObject *tp_subclasses;
    PyObject *tp_weaklist;
    destructor tp_del;

    /* Type attribute cache version tag. Added in version 2.6 */
    unsigned int tp_version_tag;

    destructor tp_finalize;
};

struct PyMethodDef {
    const char  *ml_name;   /* The name of the built-in function/method */
    PyCFunction ml_meth;    /* The C function that implements it */
    int         ml_flags;   /* Combination of METH_xxx flags, which mostly
                               describe the args expected by the C func */
    const char  *ml_doc;    /* The __doc__ attribute, or NULL */
};

struct PyMappingMethods {
    lenfunc mp_length;
    binaryfunc mp_subscript;
    objobjargproc mp_ass_subscript;
};


struct PyModuleDef_Base {
  PyObject ob_base;
  PyObject* (*m_init)(void);
  Py_ssize_t m_index;
  PyObject* m_copy;
};

struct PyModuleDef {
  PyModuleDef_Base m_base;
  const char* m_name;
  const char* m_doc;
  Py_ssize_t m_size;
  PyMethodDef *m_methods;
  struct PyModuleDef_Slot* m_slots;
  traverseproc m_traverse;
  inquiry m_clear;
  freefunc m_free;
};

#ifndef _WIN32
extern PyObject _Py_FalseStruct, _Py_TrueStruct, _Py_NoneStruct;
#endif

extern "C" {
  void _Py_Dealloc(PyObject*);
  PyObject* PyErr_Occurred();
  void PyErr_Print();
  void PyErr_SetString(PyObject*,const char*);

  
  extern PyTypeObject* PyBool_Type;
  extern PyTypeObject* PyFloat_Type;
  extern PyObject* PyExc_RuntimeError;
  
  int PyType_IsSubtype(PyTypeObject*, PyTypeObject*);
  unsigned long PyType_GetFlags(PyTypeObject*);
  PyObject* Py_GetConstantBorrowed(unsigned);
  PyObject* PyLong_FromLong(long);
  PyObject* PyFloat_FromDouble(double);
  long long PyLong_AsLongLong(PyObject*);
  double PyFloat_AsDouble(PyObject*);
  int PyType_Ready(PyTypeObject*);

  PyObject* PyObject_Str(PyObject*);
  PyObject* PyObject_Dir(PyObject*);
  PyObject* PyObject_GetAttr(PyObject*, PyObject*);
  PyObject* PyObject_GenericGetAttr(PyObject*, PyObject*);
  int PyObject_SetAttrString(PyObject*, const char*, PyObject*);
  
  int PyModule_AddObject(PyObject*, const char*, PyObject*);
  const char* PyModule_GetName(PyObject*);
  PyObject* PyModule_Create2(PyModuleDef*,int);
  int PySequence_Check(PyObject*);
  ssize_t PySequence_Size(PyObject*);
  PyObject* PySequence_GetItem(PyObject*, ssize_t i);
  
  PyObject* PyUnicode_FromString(const char*);
  char* PyUnicode_AsUTF8AndSize(PyObject*,Py_ssize_t*);

  PyObject* PyDict_New();
  int PyDict_SetItemString(PyObject* dp, const char* key, PyObject* item);
  
  PyObject* PyList_New(ssize_t size);
  int PyList_SetItem(PyObject*, ssize_t, PyObject*);
  int PyList_Append(PyObject*, PyObject*);
  
  int PyMapping_Check(PyObject*);
  PyObject* PyMapping_Items(PyObject*);
}

#endif

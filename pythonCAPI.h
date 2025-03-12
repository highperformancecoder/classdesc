/*
  @copyright Russell Standish 2025
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// Declares functions usded from the Python C API
#ifndef CLASSDESC_PYTHON_CAPI_H
#define CLASSDESC_PYTHON_CAPI_H

// TODO - is this field used on Windows?
#define _PyObject_HEAD_EXTRA
#define Py_RETURN_TRUE return Py_INCREF(Py_True), Py_True
#define Py_RETURN_FALSE return Py_INCREF(Py_False), Py_False

extern "C" {
  typedef ssize_t Py_ssize_t;
  typedef struct _object {
    _PyObject_HEAD_EXTRA
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
  } PyObject;

  PyObject* PyUnicode_FromString(const char *);
  PyObject* PyDict_New();
  
}
#endif

#include "RESTProcessExample.h"
#include "pythonBuffer.h"
#include "classdesc_epilogue.h"
//#include "RESTProcess_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si;

PyModuleDef pyExample = {
  PyModuleDef_HEAD_INIT,
    "pyExample", // Module name
    "Python interface to RESTProcess Example",
    -1,   // Optional size of the module state memory
    NULL, // Optional module methods
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

PyMODINIT_FUNC PyInit_pyExample(void)
{
  RESTProcess(registry,"root",root);
  auto module=PyModule_Create(&pyExample);
  if (module)
    {
      PyObjectRef pyRoot=CppWrapper::create("root");
      attachMethods(pyRoot,"root");
      PyModule_AddObject(module, "root", pyRoot.release());
    }
  return module;
}

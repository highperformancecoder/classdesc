#include "RESTProcessExample.h"
#include "pythonBuffer.h"
#include "classdesc_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si;


CLASSDESC_ADD_GLOBAL(root);
CLASSDESC_DECLARE_TYPE(Foo,int);
CLASSDESC_DECLARE_TYPE(Root);
CLASSDESC_PYTHON_MODULE(pyExample);


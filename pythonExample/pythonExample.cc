#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;

BOOST_PYTHON_MODULE(example)
{
  pythonObject_t p;
  pythonObject(p,"root",root);
}

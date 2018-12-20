#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si=25;

BOOST_PYTHON_MODULE(example)
{
  python_t p;
  python<Root>(p,"");
  p.addObject("static.root",root);
}


#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si=25;

void registerRoot()
{
  addPythonObject("root1",root);
  addPythonObject("example.root1",root);
}


BOOST_PYTHON_MODULE(example)
{
  python_t p;
  python<Root>(p,"");
  p.addFunctional("registerRoot1",registerRoot);
  p.addObject("static.root",root);
  p.addObject("root",root);
  addPythonObject("root",root);
}


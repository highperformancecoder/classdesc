#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;
const int Foo::csi;
int Foo::si=25;

void registerRoot()
{
  addPythonObject("root1",root);
  addPythonObject("root1",root);
  addPythonObject("example.root1",root);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Bar_overloadExample0,overloadExample,0,0);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(Bar_overloadExample1,overloadExample,1,2);

BOOST_PYTHON_MODULE(example)
{
  python_t p;
  p.defineClass<Root>();
  p.addFunctional("registerRoot1",registerRoot);
  p.addObject("static.root",root);
  p.addObject("root",root);
  addPythonObject("root",root);
  Bar& (Bar::*o1)()=&Bar::overloadExample;
  int (Bar::*o2)(int,int)=&Bar::overloadExample;
  p.getClass<Bar>().overload("overloadExample",o1);
  p.getClass<Bar>().overload("overloadExample",o2,Bar_overloadExample1(args("x","y")));
}


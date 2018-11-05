#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;

BOOST_PYTHON_MODULE(example)
{
  python_t p;
  python<Root>(p,"");
  p.addObject("static.root",root);
}


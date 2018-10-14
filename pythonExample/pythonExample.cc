#include "pythonExample.h"
#include "classdesc_epilogue.h"

Root root;

BOOST_PYTHON_MODULE(example)
{
  python_t p;
  //  python(p,"root",root);
  python<Root>(p,"");
  // TODO the following still crashes
  //p.addObject("root",root);
}

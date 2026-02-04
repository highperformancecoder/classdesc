#include <classdesc.h>
#include "c++Features.h"
#include "pack_base.h"
#include "json_pack_base.h"
#include "classdesc_epilogue.h"

using namespace classdesc;


using std::cout;
using std::endl;

int main()
{
  pack_t b;
  CppFeatures x, y;
  x.tup={10,3}; // set to different from default
  b<<x>>y;
  assert(x.tup==y.tup);

  json_pack_t jb;
  jb<<x>>y;
  assert(x.tup==y.tup);

  // make sure various C++17 types have valid typenames
  cout << typeName<std::tuple<int,int>>() << endl;
}

// why are these lines needed?? Bug in gcc?
#include <vector>
#include <unordered_map>
#include <classdesc.h>
namespace std
{
  template <class T>
  struct hash<vector<T> >
  {
    unsigned operator()(const vector<T>& x) const {
      unsigned r=0;
      for (auto i: x)
        r ^= hash<T>()(i);
      return r;
    }
  };
}

#include "pack_stl.h"
#include "c++Features.h"
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
  assert(x.tup==y.tup); // checks brace initialiser

  // make sure various C++17 types have valid typenames
  cout << typeName<std::tuple<int,int>>() << endl;
}

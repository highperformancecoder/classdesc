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

constexpr int CppFeatures::b;

using namespace classdesc;

struct Foo {};
struct Bar {};
struct FooBar {};

using std::cout;
using std::endl;

#include <vector>
template <class T> using MyVect=std::vector<T>;

int main()
{
  //  Join<Foo,Bar,FooBar> fbfb;
  pack_t b;
  CppFeatures x, y;
  x.umap.emplace(std::vector<int>(1),2);
  vect<int> v;
  x.esp=Twrap<int>(2); y.esp=Twrap<int>(3);
  b<<x>>y;
  assert(x.d==y.d); // checks brace initialiser
  assert(x.d1==y.d1); // checks = initialiser
  assert(x.esp==y.esp);
  // b<<v; //doesn't work yet

  Exclude<int*> test_nullptr=nullptr;
  assert(test_nullptr==nullptr);

  // make sure various C++11 types have valid typenames
  cout << typeName<int64_t>() << endl;
  cout << typeName<uint64_t>() << endl;
  cout << typeName<const uint64_t>() << endl;
  cout << typeName<const int64_t>() << endl;
  cout << typeName<int32_t>() << endl;
  cout << typeName<uint32_t>() << endl;
  cout << typeName<const uint32_t>() << endl;
  cout << typeName<const int32_t>() << endl;
  cout << typeName<char32_t>() << endl;
  cout << typeName<Xrefd<int,MyVect>>() << endl;
  cout << typeName<Colour>() << endl;
  for (auto& i: enum_keys<Colour>())
    cout << i.second << endl;
}

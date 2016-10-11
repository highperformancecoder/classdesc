// why are these lines needed?? Bug in gcc?
#include <vector>
#include <unordered_map>
#include <classdesc.h>
namespace std
{
  template <>
  struct is_integral<unsigned int64_t>: public true_type {};

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
  cout << typeName<unsigned int64_t>() << endl;
  cout << typeName<const unsigned int64_t>() << endl;
  cout << typeName<unsigned const int64_t>() << endl;
  cout << typeName<signed const int64_t>() << endl;
  cout << typeName<char32_t>() << endl;
  cout << typeName<Xrefd<int,MyVect>>() << endl;
}

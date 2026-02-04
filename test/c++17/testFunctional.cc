#include <function.h>
#include <classdesc_epilogue.h>

#include <iostream>
using namespace std;

using namespace classdesc;
using namespace classdesc::functional;

static_assert(is_same<typename Return<void (std::vector<std::shared_ptr<int> >::*)() noexcept>::type, void>::value);

int main()
{
  cout << typeName<typename Return<void (std::vector<std::shared_ptr<int> >::*)() noexcept>::type>() << endl;
  return 0;
}

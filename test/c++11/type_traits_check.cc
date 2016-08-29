#include "function.h"
#include "classdesc_epilogue.h"
using namespace classdesc;
using functional::AllArgs;

#include <tr1/type_traits>

struct Foo {};

int main()
{
  static_assert(!is_rvalue<string&>::value,"!is_rvalue<string&>");
  static_assert(is_rvalue<string>::value,"is_rvalue<string>");
  static_assert(is_rvalue<const string&>::value,"is_rvalue<string>");
  static_assert(is_rvalue<int>::value,"is_rvalue<int>");
  static_assert(!AllArgs<void (Foo::*)(string&,int), is_rvalue>::value,
                "!AllArgs<void (Foo::*)(string&,int), is_rvalue>");
  static_assert(AllArgs<void (Foo::*)(string,int), is_rvalue>::value,
                "AllArgs<void (Foo::*)(string,int) is_rvalue>");
  
  static_assert(AllArgs<functional::bound_method<Foo, void (Foo::*)(const string&,const string&) const>, is_rvalue>::value, "AllArgs<void (Foo::*)(const string&,const string&) const");
  static_assert(std::tr1::is_convertible<string,string>::value,"std::tr1::is_convertible<string,string>");
}

#include "function.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

struct foo
{
  void bar() {}
  void barC() {}
};

template <class C, class M> typename
enable_if<And<is_const<C>,Not<functional::is_const_method<M> > >, void>::T
callMethod(C& o, M m) 
{/* don't do anything - can't call nonconst method on a const object*/}

template <class C, class M> typename
enable_if<Or<Not<is_const<C> >, functional::is_const_method<M> >, void>::T
callMethod(C& o, M m) 
{(o.*m)();}

int main()
{
  foo x;
  callMethod(x,&foo::bar);
  callMethod(x,&foo::barC);
  const foo y;
  callMethod(y,&foo::bar);
  callMethod(y,&foo::barC);
}

struct Foo
{
  void bar1() {}
  void bar2() const {}
};

#include <function.h>
#include <classdesc_epilogue.h>
using classdesc::functional::bindMethod;

template <class F>
void call(F f) {f();}

int main()
{
  Foo f;
  call(bindMethod(f,&Foo::bar1));
  call(bindMethod(f,&Foo::bar2));
  const Foo cf;
#if __cplusplus>=201103L
  // TODO - support bindMethod on non constant methods to constant
  // objects in legacy C++?
  call(bindMethod(cf,&Foo::bar1));
#endif
  call(bindMethod(cf,&Foo::bar2));
}

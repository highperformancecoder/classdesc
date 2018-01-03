/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "classdesc.h"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace classdesc;

struct Foo
{
  int a;
  Foo(int x=0): a(x) {}
  void b() {}
  Foo operator+(const Foo& x) const {return a+x.a;}
};

int main()
{
  Exclude<int> eint;
  
  Exclude<Foo> efoo(2);
  Exclude<Foo*> epfoo(&efoo);
  Exclude<int*> epint(&(int&)eint);
  Exclude<bool> bfoo(false);

  eint = 3;
  assert(eint+2 == 5);
  assert(2+eint == 5);
  assert(*epint+2 ==5);
  assert(2+*epint ==5);
  assert((efoo+Foo(3)).a==5);
  assert((Foo(3)+efoo).a==5);
  assert((*epfoo+Foo(3)).a==5);
  efoo.b();
  epfoo->b();
  assert( (epfoo+2) - epfoo == 2);
  assert( (2+epfoo) - epfoo == 2);

  assert(bfoo||true);
  assert((!bfoo)&&true);
  assert(!(bfoo&&true));
  assert((bfoo||bfoo)==false);
  assert((bfoo&&bfoo)==false);
  assert(bfoo==bfoo);
  assert(!(bfoo!=bfoo));
  bfoo|=true;
  assert(bfoo);
  bfoo&=false;
  assert(!bfoo);
}

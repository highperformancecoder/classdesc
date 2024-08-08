/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "poly.h"
//#include "ref.cd"
#include "testRef.h"
#include "testRef.cd"
#include "classdesc_epilogue.h"
using namespace classdesc;

struct bar
{
  int x;
  bar(int x=0): x(x) {}
};

int main()
{
  poly<AbstractObject> a=Object1();
  assert(a->type()==0);
  a.addObject<Object2>();
  assert(a->type()==1);

  polyref<AbstractObject> b, c;
  assert(!b);
  b.addObject<Object2>();
  assert(b->type()==1);
  c=b;
  assert(b);
  assert(&*b==&*c); //test that it is the same object

  void (Object2::*m)() = &Object2::foo;
  ref<Object2> r;
  (r->*m)();

  {
    // test copy and assignment from bare objects
    ref<bar> a(bar(1));
    assert(a.refCount()==1 && a->x==1);
    ref<bar> b;
    assert(b.refCount()==0);
    b=a;
    assert(b.refCount()==2 && b->x==1);
    b=bar(2);
    assert(b.refCount()==1 && b->x==2);
  }
}

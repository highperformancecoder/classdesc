/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/*
  Test things like the equality operator and streaming is reversible
*/

#include "javaClass.h"
#include "javaClass_serialisation.h"
#include "javaClass.cd"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace classdesc;

int main()
{
  ClassFile cf1, cf2;
  assert(cf1==cf2);
  cf1.super_class=3;
  assert(!(cf1==cf2));

  pack_t b1("Foo.class","r"), b2;
  b1 >> cf1;
  b2 << cf1 >> cf2;
  assert(cf1==cf2);
}

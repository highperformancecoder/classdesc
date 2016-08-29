/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "nestedClass.h"
#include "nestedClass.cd"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace classdesc;

int main()
{
  foo::bar tmp1, tmp2;
  tmp1.a=2;
  pack_t b;
  b<<tmp1;
  b>>tmp2;
  assert(tmp1.a==tmp2.a);
}


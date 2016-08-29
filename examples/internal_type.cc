/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/* this is more of a test, than an illustrative example */

#include "internal_type.h"
#include "internal_type.cd"
using namespace classdesc;

#include "classdesc_epilogue.h"

int main()
{
  foo<int> a;
  pack_t t;
  t<<a;
  return 0;
}

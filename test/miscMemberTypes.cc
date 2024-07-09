/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "pack_base.h"
#include "pack_stl.h"
#include "miscMemberTypes.h"
#include "classdesc_epilogue.h"

using namespace classdesc;

int main()
{
  Foo foo;
  Bar<int> bar;
  pack_t buf;
  buf<<foo<<bar>>foo>>bar;
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "testExclude.h"
#include <classdesc_epilogue.h>
#include <assert.h>

int main()
{
  pack_t b;
  foo f;
  f.unpackable=new bar;
  f.unpackable->barfoo();
  f.foosbar.barfoo();
  b<<f;
  b>>f;
  return 0;
}

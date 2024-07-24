/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "testExclude.h"
#include <classdesc_epilogue.h>
#include <assert.h>
#include <memory>
using namespace std;

int main()
{
  pack_t b;
  foo f;
  shared_ptr<bar> barPtr(new bar);
  f.unpackable=barPtr.get();
  f.unpackable->barfoo();
  f.foosbar.barfoo();
  b<<f;
  b>>f;
  return 0;
}

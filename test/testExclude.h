/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <classdesc.h>
using namespace classdesc;

struct bar
{
  void barfoo() {}
};

struct foo
{
  Exclude<bar*> unpackable;
  Exclude<bar> foosbar;
};

#include "testExclude.cd"

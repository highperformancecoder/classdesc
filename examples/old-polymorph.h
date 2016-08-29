/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "poly.h"
using namespace classdesc;


struct foo: public Object<foo>
{
  int x;
};

struct bar: public Object<bar,foo>
{
  int y,z;
  bool operator!=(const bar& a) const {return a.x!=x || a.y!=y || a.z!=z;}
};


/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "old-polymorph.h"
#include "old-polymorph.cd"
#include "object.cd"
#include "pack_stream.h"
#include "classdesc_epilogue.h"

#include <stdio.h>

using namespace classdesc;

namespace R
{
  Register<foo> rfoo;
  Register<bar> rbar;
}

void pack(pack_t& b, struct foo& x) {x.pack(b);}
void unpack(pack_t& b, struct foo& x) {x.unpack(b);}

int main()
{
  pack_t buf;
  ref<poly<foo> > aa, bb;
  aa->addObject<bar>();
  bar& a=aa->cast<bar>();
  a.x=1; a.y=2; a.z=3;
  buf << aa >> bb;
  if (aa->cast<bar>()!=bb->cast<bar>())
    {
      puts("polymorphism failed!");
      return 1;
    }
  return 0;
}

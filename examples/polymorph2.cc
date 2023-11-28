/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "polymorph2.h"
#include "classdesc_epilogue.h"

#include <sstream>
#include <stdio.h>

using namespace classdesc;
using namespace std;

// factory method
object* object::create(const string& nm)
{
  return factory.create(nm);
}

namespace classdesc
{
  // register types in the factory
  template <> Factory<object, string>::Factory()
  {
    registerType<foo>();
    registerType<bar>();
  }
}

Factory<object, string> factory;

struct xml_packunpack
{
  stringstream s;
  xml_pack_t pb;
  xml_unpack_t ub;
  xml_packunpack(): pb(s) {}
  template <class T>
  xml_packunpack& operator<<(T& x)
  {pb << x; ub.clear(); ub.parse(s); return *this;}
  template <class T>
  xml_packunpack& operator>>(T& x)
  {ub >> x; return *this;}
};

template <class B>
bool test(B& buf)
{
  bar* a=new bar;
  classdesc::shared_ptr<object> aa(a), bb;
  a->x=1; a->y=2; a->z=3;
  buf << aa >> bb;
  bar *b=dynamic_cast<bar*>(bb.get());
  return b && *a==*b;
}
        
int main()
{
  pack_t buf;
  xml_packunpack xbuf;
  if (test(buf) && test(xbuf))
    return 0;
  puts("polymorphism failed!");
  return 1;
}

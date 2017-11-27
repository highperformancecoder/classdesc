/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "json_pack_base.h"
#include "classdesc_epilogue.h"
using namespace classdesc;
using namespace std;

class foo
{
};

ostream& operator<<(ostream& o, foo& a)
{
  o<<"hello_from_foo";
  return o;
}

istream& operator>>(istream& i, foo& a)
{
  std::string s;
  i>>s;
  assert(s=="hello_from_foo");
  return i;
}

int main()
{
json_unpack_t b;
foo a;
b<<a;
b>>a;
}

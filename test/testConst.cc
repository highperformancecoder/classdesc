/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "testConst.h"
#include "testConst.cd"
#include "classdesc_epilogue.h"
#include <assert.h>
#include <string>
using namespace std;

const int Foo::static_const_b;

int main()
{
  Foo foo1, foo2;
  classdesc::pack_t b;
  b << foo1 >> foo2;

  classdesc::json_pack_t j;
  j << foo1 >> foo2;
  string json = json_spirit::write(j);
  assert(json.find("const_a")!=string::npos);
  assert(json.find("static_const_b")==string::npos);

  ostringstream o;
  classdesc::xml_pack_t xp(o);
  xp << foo1;
  assert(o.str().find("const_a")!=string::npos);
  assert(o.str().find("static_const_b")==string::npos);
  
  istringstream i(o.str());
  classdesc::xml_unpack_t xup(i);
  xup >> foo2;
  
}

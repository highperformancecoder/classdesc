/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "json_pack_base.h"
#include "testXMLcd.h"
#include "testXMLcd.cd"
#include "classdesc_epilogue.h"
using namespace std;
using namespace classdesc;
using namespace json5_parser;

int main()
{

  {
    Bar bar1(1), bar2;
    bar1.iex=1; bar2.iex=2;
    // test valid shared ptr serialises correctly
    bar1.sef.reset(new EnumFoo(ec));
    {
      ofstream f1("bar.jsontmp");
      json_pack_t x1;
      x1 << bar1;
      write_formatted(x1,f1);
    }
    ifstream f1("bar.jsontmp");
    json_unpack_t x2;
    read(f1,x2);
    x2 >> bar2;
    assert(bar2.iex==2);
    if (bar1!=bar2) return 1;
  }

  Bar1 bar1(1), bar2;
  // test invalid shared ptr serialises correctly
    {
      ofstream f1("bar1.jsontmp");
      json_pack_t x1;
      x1 << bar1;
      write_formatted(x1,f1);
    }  
    ifstream f1("bar1.jsontmp");
    json_unpack_t x2;
    read(f1,x2);
    x2>>bar2;
    if (bar1!=bar2) return 2;

}
  

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "testXMLcd.h"
#include "testXMLcd.cd"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace std;
using namespace classdesc;

int main()
{
  xml_unpack_t x;
  istringstream is("<root><c1><string>&#13;</string><string>&#13;</string></c1></root>");
  x.parse(is);
  x.printContentMap();

  {
    map<int,int> a, b;
    a[1]=3; a[5]=2;
    ostringstream o;
    xml_pack_t x1(o);
    x1<<a;
    std::string ii=o.str();
    istringstream i(ii);
    xml_unpack_t x2(i);
    x2>>b;
    assert(a==b);
  }
  {
    Bar bar1(1), bar2;
    bar1.iex=1; bar2.iex=2;
    // test valid shared ptr serialises correctly
    bar1.sef.reset(new EnumFoo(ec));
    {
      ofstream f1("bar.xmltmp");
      xml_pack_t x1(f1);
      x1 << bar1;
    }  
    xml_unpack_t x2("bar.xmltmp");
    x2>>bar2;
    if (bar1!=bar2) return 1;
    assert(bar2.iex==2);
  }

  Bar1 bar1(1), bar2;
  // test invalid shared ptr serialises correctly
    {
      ofstream f1("bar1.xmltmp");
      xml_pack_t x1(f1);
      x1 << bar1;
    }  
    xml_unpack_t x2("bar1.xmltmp");
    x2>>bar2;
    if (bar1!=bar2) return 2;

    //test serialisation of xml_unpack_t objects
    pack_t buf;
    xml_unpack_t x3;
    buf<<x2 >> x3;
    x3 >> bar1;
    if (bar1!=bar2) return 3;
}
  

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// copy of testXMLcd to check that everything is correctly inlined to
// avoid multiple symbol failures at link time.

#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "xml_unpack_base.cd"
#include "testXMLcd.h"
#include "testXMLcd.cd"
#include "classdesc_epilogue.h"
using namespace std;
using namespace classdesc;

int other()
{
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
    {
      ofstream f1("bar.xmltmp");
      xml_pack_t x1(f1);
      x1 << bar1;
    }  
    xml_unpack_t x2("bar.xmltmp");
    x2>>bar2;
    if (bar1!=bar2) return 1;
  }

  Bar1 bar1(1), bar2;
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
  return 0;
}

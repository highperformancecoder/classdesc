/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "xml_unpack_base.cd"
#include "testXMLcd.h"
#include "testXMLcd.cd"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace std;
using namespace classdesc;

int main()
{
  xsd_generate_t g;
  Root r;
  string rootTag="root";
  xsd_generate(g, rootTag, r);

  ofstream f("test1.xsd");

  string schema="schema";
  g.output(f,schema);
  f.close();
  f.open("test1.xml");
  xml_pack_t x(f,schema);
  x.prettyPrint=true;
  xml_pack(x,rootTag,r);
}
  

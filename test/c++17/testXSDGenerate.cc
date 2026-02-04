#include "c++Features.h"
#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "xsd_generate_base.h"
#include "classdesc_epilogue.h"
#include <assert.h>
using namespace std;
using namespace classdesc;

int main()
{
  xsd_generate_t g;
  CppFeatures x;
  x.tup={10,3}; // set to different from default
  string rootTag="root";
  xsd_generate(g, rootTag, x);

  ofstream f("test1.xsd");

  string schema="schema";
  g.output(f,schema);
  f.close();
  f.open("test1.xml");
  xml_pack_t xb(f,schema);
  xb.prettyPrint=true;
  xml_pack(xb,rootTag,x);

  // check roundtrip serialisation
  CppFeatures y;
  xml_unpack_t ux("test1.xml");
  ux>>y;
  assert(x.tup==y.tup);
}
  

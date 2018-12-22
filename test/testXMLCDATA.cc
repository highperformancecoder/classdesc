#include "testXMLCDATA.h"
#include "testXMLCDATA.cd"
#include <classdesc_epilogue.h>

#include <ostream>
#include <sstream>
#include <assert.h>
using namespace std;
using namespace classdesc;

int main()
{
  Foo x,y;
  x.bar="<some>random &amp; <xml/></some>";
  ostringstream os;
  xml_pack_t xp(os);
  xp << x;

  //cout <<os.str() << endl;

  xml_unpack_t xup;
  istringstream is(os.str());
  xup.parse(is);
  xup >> y;
  assert(x.bar==y.bar);

  // check processing of CDATA within elements
  istringstream is2("<root><bar>Some data with <![CDATA[&amp;[]>]]> stuff</bar></root>");
  xup.clear();
  xup.parse(is2);
  xup >> y;
  assert(y.bar=="Some data with &amp;[]> stuff");
}

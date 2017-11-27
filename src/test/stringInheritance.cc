#include "stringInheritance.h"
#include <classdesc_epilogue.h>
#include <sstream>
using namespace std;
using namespace classdesc;
int main()
{
  {
    ostringstream o;
    xml_pack_t x(o);
    Foo f, f1;
    (string&)f="hello"; f.bar=1;
    x<<f;

    cout << o.str() << endl;
    istringstream is(o.str());
    xml_unpack_t x1(is);
    x1>>f1;
    assert(deepEq(f,f1));
  }
  // now with pretty printing
  {
    ostringstream o;
    xml_pack_t x(o);
    Foo f, f1;
    (string&)f="hello"; f.bar=1;
    x.prettyPrint=true;
    x<<f;

    istringstream is(o.str());
    xml_unpack_t x1(is);
    x1>>f1;
    assert(deepEq(f,f1));
  }
  // test of this case with JSON
  {
    json_pack_t x;
    Foo f, f1;
    (string&)f="hello"; f.bar=1;
    x<<f;
    write_formatted(x,cout);
    x>>f1;
    cout<<endl;
    assert(deepEq(f,f1));
  }
}

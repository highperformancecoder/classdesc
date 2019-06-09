#include <array>
#include "pack_base.h"
#include "json_pack_base.h"
#include "xml_pack_base.h"
#include "xsd_generate_base.h"
#include "dump_base.h"
#include "random_init_base.h"
#include "classdesc_epilogue.h"
using namespace classdesc;
using namespace std;

struct xmlBuf
{
  ostringstream os;
  xml_pack_t xp{os};
  template <class T>
  xmlBuf& operator<<(const T& x) {xp<<x; return *this;}
  template <class T>
  void operator>>(T& x) {
    istringstream is(os.str());
    xml_unpack_t xu(is);
    xu>>x;
  }
};



  template <class B>
void check()
{
  array<int,5> a1={1,2,3,4,5}, a2={0,0,0,0,0};
  B b1,b2,b3;
  b1<<a1>>a2;
  assert(a1==a2);
  array<int,3> a3;
  b2<<a1>>a3;
  assert((a3==array<int,3>{1,2,3}));
  array<int,7> a4={0,0,0,0,0,0,0};
  b3<<a1>>a4;
  assert((a4==array<int,7>{1,2,3,4,5,0,0}));
}  


int main()
{
  check<pack_t>();
  check<json_pack_t>();
  check<xmlBuf>();

  array<int,5> a={1,2,3,4,5};

  {
    ostringstream is;
    xsd_generate_t xsd;
    xsd_generate(xsd,"root",a);
    xsd.output(is,"");
    cout << is.str() << endl;
  }
  {
    ostringstream is;
    dump_t d(is);
    dump(d,"",a);
    cout << is.str() << endl;
  }
  
  random_init_t r;
  random_init(r,"",a);
  for (auto i: a)
    cout << i<<" ";
  cout << endl;

}

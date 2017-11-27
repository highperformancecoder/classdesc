#include <vector>
#include <map>
#include <string>
#include <sstream>
using namespace std;

#include "pack_base.h"
#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "json_pack_base.h"
#include "classdesc_epilogue.h"
using namespace classdesc;

// tests that containers are cleared before being unpacked into

int main()
{
  map<int,int> m, m1;
  vector<int> v(2), v1;

  m[0]=0; m[1]=1;

  {
    pack_t x;
    pack(x,"",m);
    m1[3]=2;
    unpack(x,"",m1);
    assert(m1.size()==m.size());
  }
  
  {
    pack_t x;
    pack(x,"",v);
    v1.resize(1);
    unpack(x,"",v1);
    assert(v1.size()==v.size());
  }
  
  {
    json_pack_t x;
    json_pack(x,"",m);
    m1[3]=2;
    json_unpack(x,"",m1);
    assert(m1.size()==m.size());
  }
  
  {
    json_pack_t x;
    json_pack(x,"",v);
    v1.resize(1);
    json_unpack(x,"",v1);
    assert(v1.size()==v.size());
  }
  
  {
    ostringstream os;
    xml_pack_t x(os);
    os.flush();
    xml_pack(x,"m",m);
    m1[3]=2;
    istringstream is(os.str());
    xml_unpack_t u(is);
    xml_unpack(u,"m",m1);
    assert(m1.size()==m.size());
  }
   
  {
    ostringstream os;
    xml_pack_t x(os);
    xml_pack(x,"v",v);
    v1.resize(1);
    istringstream is(os.str());
    xml_unpack_t u(is);
    xml_unpack(u,"v",v1);
    assert(v1.size()==v.size());
  }
  
 

  
  
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

struct foo
{
  int a;
  double b;
  bool operator==(struct foo& x) {return a==x.a && b==x.b;}
};

#include "pack_base.h"
#include "pack_stl.h"
#include "classdesc_epilogue.h"
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
using namespace classdesc;
using namespace std;
#include <assert.h>

bool test(BinStream& bs)
{
  foo a={1,3}, b;
  bs << a >> b;
  return a==b;
}

int main()
{
  pack_t p;
  BinStream pack_bs(p);
  assert(test(pack_bs));
  xdr_pack x;
  BinStream xdr_pack_bs(x);
  assert(test(xdr_pack_bs));

  BinStreamT<pack_t> template_pack_bs;
  assert(test(template_pack_bs));

  BinStreamT<xdr_pack> template_xdr_pack_bs;
  assert(test(template_xdr_pack_bs));

  {
    int a[3]={1,2,3}, b[3];
    pack_bs << a >> b;
    assert(memcmp(a,b,3*sizeof(int))==0);
  }

  {
    vector<int> a(10,2), b;
    pack_bs << a >> b;
    assert(a==b);
  }

  {
    list<int> a(10,2), b;
    pack_bs << a >> b;
    assert(a==b);
  }

  {
    map<int, int> a, b;
    a[1]=2;
    a[3]=4;
    pack_bs << a >> b;
    assert(a==b);
  }
  // Finally test arguments to BinStreamT
  {
    vector<int> a(10,2), b;
    {
      BinStreamT<pack_t> w("foo.dat","w");
      w << a;
    }
    BinStreamT<pack_t> r("foo.dat","r");
    r >> b;
    assert(a==b);
  }
 
}

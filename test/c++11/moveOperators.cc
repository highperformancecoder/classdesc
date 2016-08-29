#include "pack_base.h"
#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "json_pack_base.h"
#include "classdesc_epilogue.h"

#include <iostream>
#include <assert.h>

using namespace classdesc;
using namespace std;

// tests whether move operators are correctly implemented

int main()
{
  {
    pack_t b(200);
    pack_t b1=move(b);
    assert(b.data()==nullptr);
    b1=pack_t(300);
  }

  {
    xdr_pack b(200);
    xdr_pack b1=move(b);
    assert(b.data()==nullptr);
    b1=xdr_pack(300);
  }

  {
    xml_pack_t b(cout);
    xml_pack_t b1=move(b);
    b1=xml_pack_t(cout);
  }

   {
    xml_unpack_t b;
    xml_unpack_t b1=move(b);
    b1=xml_unpack_t();
  }

 {
    json_pack_t b;
    json_pack_t b1=move(b);
    b1=json_pack_t();
  }
return 0;
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "pack_base.h"
#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "json_pack_base.h"
using namespace classdesc;
#include "testJsonSharedPtr.h"
#include "testJsonSharedPtr.cd"
#include "classdesc_epilogue.h"
#include <assert.h>

#include <memory>

base* base::create(int t)
{
  switch (t)
    {
    case 1: return new subclass;
    default: return 0;
    }
}

template <class T, class B>
void testPtr()
{
  B buf;
  subclass *aa=new subclass;
  T a(aa), b;
  aa->a=1;
  buf <<  a >> b;
  subclass *bb=dynamic_cast<subclass*>(b.get());
  assert(bb && bb->a==1);
}

struct xml_packunpack
{
  xml_pack_t pb;
  xml_unpack_t ub;
  std::stringstream s;
  xml_packunpack(): pb(s) {}
  template <class T>
  xml_packunpack& operator<<(T& x)
  {pb << x; ub.clear(); ub.parse(s); return *this;}
  template <class T>
  xml_packunpack& operator>>(T& x)
  {ub >> x; return *this;}
};

int main()
{
  json_pack_t buf;
  foo a(new int(1), 0), b(0, new int);
  buf <<  a >> b;
  assert(a.a && b.a && *a.a==*b.a);
  assert(!a.b && !b.b);

  testPtr<shared_ptr<base>,pack_t>();
  testPtr<shared_ptr<base>,xml_packunpack>();
  testPtr<shared_ptr<base>,json_pack_t>();
#if defined(__cplusplus) && __cplusplus>=201103L
  testPtr<std::unique_ptr<base>,pack_t>();
  testPtr<std::unique_ptr<base>,xml_packunpack>();
  testPtr<std::unique_ptr<base>,json_pack_t>();
#else
  testPtr<std::auto_ptr<base>,pack_t>();
  testPtr<std::auto_ptr<base>,xml_packunpack>();
  testPtr<std::auto_ptr<base>,json_pack_t>();
#endif
}

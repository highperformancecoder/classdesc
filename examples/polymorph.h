/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "polyBase.h"
#include "polyPackBase.h"
#include "polyXMLBase.h"
#include "factory.h"
#include "xml_pack_base.h"
using namespace classdesc;

// base type
struct object: public PolyBase<string>, virtual public PolyPackBase, virtual public PolyXMLBase
{
  static object* create(const string&);
};

// typeid template 

/* Mixins work well with flat heirarchies, but when objects are
 derived from others, you can run into "no unique final overrider"
 errors. There doesn't seem to be a way of specifying which final
 override you want. I have two possible solutions, neither of which
 I'm particularly happy with. The solution here involves
 reimplementing the curiously recursive template pattern, and not
 using the mixins. Alternatively, you can use mixins, but append the
 heirarchy base afterwards.
*/
template <class T, class S=object>
struct Object: public S
{
  string type() const {return typeName<T>();}
  Object* clone() const {return new T(*static_cast<const T*>(this));}
  void pack(pack_t& x, const string& d) const
  {::pack(x,d,static_cast<const T&>(*this));}
  void unpack(unpack_t& x, const string& d)
  {::unpack(x,d,static_cast<T&>(*this));}
  void xml_pack(xml_pack_t& x, const string& d) const
  {::xml_pack(x,d,static_cast<const T&>(*this));}
  void xml_unpack(xml_unpack_t& x, const string& d)
  {::xml_unpack(x,d,static_cast<T&>(*this));}
};


struct foo: public Object<foo>
{
  int x;
};

struct bar: public Object<bar, foo>
{
  int y,z;
  bool operator==(const bar& a) const {return a.x==x && a.y==y && a.z==z;}
};

extern Factory<object, string> factory;

#include "polymorph.cd"

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
 I'm particularly happy with. The solution here involves decorating the classes with the base class after class definition, so that the base class only appears once in the inheritance tree. 
 
*/
template <class T>
struct Object: 
  public Poly<T,object>, 
  public PolyPack<T>, 
  public PolyXML<T> 
{
  string type() const {return typeName<T>();}
};


struct foo_b
{
  int x;
};

struct foo: public foo_b, public Object<foo> {};

struct bar_b: public foo_b
{
  int y,z;
  bool operator==(const bar_b& a) const {return a.x==x && a.y==y && a.z==z;}
};

struct bar: public bar_b, public Object<bar> {};

extern Factory<object, string> factory;

#include "polymorph2.cd"

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "polyBase.h"
#include "polyPackBase.h"
#include "polyXMLBase.h"
#include "polyJsonBase.h"

struct foo
{
  shared_ptr<int> a, b;
  foo(int *a, int* b): a(a), b(b) {}
};

// test polymorphic serialisation
struct base: virtual public PolyBase<int>, virtual public PolyPackBase, 
             virtual public PolyXMLBase, virtual public PolyJsonBase
{
  static base* create(int t);
};

struct subclass: public Poly<subclass, base>, public PolyPack<subclass>, 
                 public PolyXML<subclass>,  public PolyJson<subclass> 
{
  int type() const {return 1;}
  int a;
  subclass(): a(0) {}
};


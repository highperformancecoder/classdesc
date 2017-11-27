/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

using classdesc::object;
using classdesc::Object;
#include <stdio.h>

struct AbstractObject: public object 
{
  virtual void foo() {}
};
struct Object1: public Object<Object1, AbstractObject> 
{
  void foo() {}
};
struct Object2: public Object<Object2, AbstractObject> 
{
  void foo() {puts("foo2");}
};


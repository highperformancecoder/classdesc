/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <iostream>
using namespace std;

class abc
{
 public:
  virtual void bar()=0;
};

class foo1 
{
public:
  virtual void bar() {}
};
class foo2 
{
public:
  virtual void bar() {}
};
class foo3: public foo1 {};
class foo4: public foo2 {};

#include "test_isa.cd"
#include "classdesc_epilogue.h"

int main()
{
  foo3 x; foo4 y;
  return isa(x,foo1()) && !isa(x,foo2()) && !isa(y,foo1()) && isa(y,foo2());
}

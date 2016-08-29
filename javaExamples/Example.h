/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "classdesc_access.h"
 #include <iostream>

struct Example
{
  int foo;
  float bar;
  void printFoo() {std::cout << "foo="<<foo << std::endl; }
  void printBar() {std::cout << "bar="<<bar << std::endl; }
};

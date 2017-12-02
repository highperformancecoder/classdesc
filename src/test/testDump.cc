/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "testXMLcd.h"
#include "testXMLcd.cd"
#include "classdesc_epilogue.h"
using classdesc::operator<<;

int main()
{
  Bar bar(0);
  //  std::cout << bar << std::endl;
  dump(std::cout,"bar",bar);
  std::cout<<std::endl;
}

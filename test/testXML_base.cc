/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "xml_unpack_base.h"
#include <fstream>
#include "classdesc_epilogue.h"

int main()
{
  classdesc::xml_unpack_t X(std::cin);
  X.printContentMap();

  {
    // check that various boolean cases are handled
    std::string data="<x><a>1</a><b>t</b><c>TRUE</c>"
      "<d>Y</d><e>yes</e><f>ON</f><g>false</g><h>0</h></x>";
    std::istringstream i(data);
    classdesc::xml_unpack_t x(i);
    bool b;
    assert((x.unpack("x.a",b), b));
    assert((x.unpack("x.b",b), b));
    assert((x.unpack("x.c",b), b));
    assert((x.unpack("x.d",b), b));
    assert((x.unpack("x.e",b), b));
    assert((x.unpack("x.f",b), b));
    assert((x.unpack("x.g",b), !b));
    assert((x.unpack("x.h",b), !b));
 }
  return 0;
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "javaClass.h"
#include "javaClass_base.h"
#include "javaClass_serialisation.h"
#include "javaClass.cd"
#include "classdesc_epilogue.h"

using namespace classdesc;

int main(int argc, char* argv[])
{
  if (argc<2)
    {
      puts("please supply a Java class file");
      return 1;
    }
  pack_t buf(argv[1],"r");
  ClassFile cf;
  buf >> cf;
  dump(std::cout, argv[1], cf);
}

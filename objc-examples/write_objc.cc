/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "writeobjc_base.h"

#include FILE_NAME(CNAME,.h)
#include FILE_NAME(CNAME,.cd)

int main()
{  writeobjc_t targ;
   CNAME arg;
   WRITE_OBJC(CNAME);
   return 0;
}

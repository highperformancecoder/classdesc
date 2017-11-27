/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "ticket20-example.h"
#include "ticket20-example.cd"
#include <classdesc_epilogue.h>
using namespace minsky;
using namespace classdesc;

int main() 
{
  Msg<PayloadPtr<int> > a;
  pack_t b;
  b << a;
  return 0;
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "javaClass.h"
#include "javaClass_serialisation.h"
#include "javaClass.cd"
#include "javaClass_base.h"
#include "javaClass_base.cd"
#include "Example.h"
#include "Example.cd"
#include "object.cd"
#include "pack_stream.h"
#include "dump_epilogue.h"
#include "classdesc_epilogue.h"

using namespace classdesc;

int main()
{
  javaClass_t cf, cf1;
  Example ex;
  javaClass(cf,"ex",ex);
  cf.access_flags |= JVM_ACC_INTERFACE; //we only need interfaces
  dump(std::cout,"ex",cf);
  pack_t b;
  b << cf >> cf1;
  assert(cf == cf1);
}

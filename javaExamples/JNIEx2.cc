/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "Example.h"
#include "Example.cd"
#include "classdesc_epilogue.h"

#include "JNIEx.h"

namespace
{
  classdesc::JNIRegister<Example> example("Example");
}

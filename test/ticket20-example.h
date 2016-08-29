/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "classdesc.h"

namespace minsky
{
  template <class Payload>
  struct PayloadPtr {};

  template <class Payload> struct Msg;

  template <class Payload>
  struct Msg<minsky::PayloadPtr<Payload> >
  {
    Msg() {}
  };

}

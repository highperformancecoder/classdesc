/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef SIGNATURE_H
#define SIGNATURE_H
#include <string>
#include <vector>
namespace classdesc
{
  /// Represents the signature of a function - serialisable to JSON or XML.
  /// String values are those given by typeName
  struct Signature
  {
    std::string ret;
    std::vector<std::string> args;
  };
}

#include "signature.cd"

#pragma omit RESTProcess classdesc::Signature
namespace classdesc_access
{
#ifdef RESTPROCESS_H // spurious, needed for old Classdesc
  template <>
  struct access_RESTProcess<classdesc::Signature>: public classdesc::NullDescriptor<classdesc::RESTProcess_t> {};
#endif
}

#endif

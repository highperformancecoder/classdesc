/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_SIGNATURE_H
#define CLASSDESC_SIGNATURE_H
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
    bool operator==(const Signature& s) const {return args==s.args;}
  };
}

#endif

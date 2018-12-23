/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef XML_COMMON_H
#define XML_COMMON_H
namespace classdesc
{
  /// utility for generating index keys (for use with arrays)
  inline std::string idx(const std::string& prefix, size_t i)
  {
    std::ostringstream s;
    s<<prefix<<"#"<<i;
    return s.str();
  }

  /// used to transfer contents of CDATA sections.
  struct CDATA: public std::string
  {
    using std::string::operator=;
  };

  class xml_pack_t;
  class xml_unpack_t;
}

#ifdef _CLASSDESC
#pragma omit xml_pack classdesc::CDATA
#pragma omit xml_unpack classdesc::CDATA
#endif

#include "xml_common.cd"
#endif

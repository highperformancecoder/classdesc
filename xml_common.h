/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_XML_COMMON_H
#define CLASSDESC_XML_COMMON_H
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
    CDATA() {}
    template <class T> CDATA(T x): std::string(x) {}
    template <class T, class U> CDATA(T x, U y): std::string(x,y) {}
  };

  class xml_pack_t;
  class xml_unpack_t;
}

#ifdef _CLASSDESC
#pragma omit xml_pack classdesc::CDATA
#pragma omit xml_unpack classdesc::CDATA
#endif

namespace classdesc_access
{
  template <class T> struct access_xml_pack;
  template <class T> struct access_xml_unpack;
}

#endif

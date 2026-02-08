/*
  @copyright Russell Standish 2016
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_STRINGKEYMAP_H
#define CLASSDESC_STRINGKEYMAP_H
#include <map>
#include <string>
#include "classdesc.h"
#if defined(__cplusplus) && __cplusplus>=201103L
#include "json_pack_base.h"
#endif
#include "xml_pack_base.h"
#include "xml_unpack_base.h"
#include "xsd_generate_base.h"

namespace classdesc
{
  /// A specialisation of a map with string keys, allowing JSON and
  /// XML objects to be mapped directly
  template <class T>
  struct StringKeyMap: public std::map<std::string, T> {};

  template <class T>
  struct is_associative_container<classdesc::StringKeyMap<T> >: public true_type {};

  template <class T> struct is_stringKeyMap<classdesc::StringKeyMap<T> >: public true_type {};

}

#ifdef _CLASSDESC
#pragma omit json_pack classdesc::StringKeyMap
#pragma omit json_unpack classdesc::StringKeyMap
#pragma omit xml_pack classdesc::StringKeyMap
#pragma omit xml_unpack classdesc::StringKeyMap
#pragma omit xsd_generate classdesc::StringKeyMap
#endif
#endif

/*
  @copyright Russell Standish 2016
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef STRINGKEYMAP_H
#define STRINGKEYMAP_H
#include <map>
#include <string>
#include "classdesc.h"
#include "json_pack_base.h"
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

namespace classdesc
{
  template <class T>
  void convert(StringKeyMap<T>& map,  const json_pack_t& j)
  {j>>map;}

  template <class T>
  void json_pack_stringKeyMap(json_pack_t& j,const string& d, const StringKeyMap<T>& a)
  {
    try
      {
        json_spirit::mValue& parent=json_find(j,head(d));
        if (parent.type()!=json_spirit::obj_type)
          throw json_pack_error("attempt to pack an array member into a non-object");
        else
          {
            typename StringKeyMap<T>::const_iterator i=a.begin();
            for (; i!=a.end(); ++i)
              json_pack(j,d+"."+i->first, i->second);
          }
      }
    catch (json_pack_error&)
      {
        if (j.throw_on_error) throw;
      }
  }

  template <class T>
  void json_unpack_stringKeyMap(json_pack_t& j,const string& d, StringKeyMap<T>& a)
  {
    try
      {
        const json_spirit::mValue& val=json_find(j,d);
        if (val.type()!=json_spirit::obj_type)
          throw json_pack_error("%s is not an array",d.c_str());
        else
          {
            const json_spirit::mObject& arr=val.get_obj();
            a.clear();
            for (json_spirit::mObject::const_iterator i=arr.begin(); i!=arr.end(); ++i)
              json_unpack(j,d+"."+i->first,a[i->first]);
          }
      }
    catch (json_pack_error&)
      {
        if (j.throw_on_error) throw;
      }
  }

  template <class T>
  void xsd_generate(xsd_generate_t& g, const string& d, const StringKeyMap<T>& a)
  {
    std::ostringstream os;
    // element name is given by the type name
    string eName=typeName<T>().c_str();
    eName=eName.substr(0,eName.find('<')); //trim off any template args
    // strip leading namespace and qualifiers
    const char *el=eName.c_str()+eName.length();
    while (el!=eName.c_str() && *(el-1)!=' ' && *(el-1)!=':') el--;

    string type=transformTypeName(typeName<StringKeyMap<T> >());
    os << "  <xs:complexType name=\"" << type << "\">\n";
    os << "    <xs:sequence minOccurs=\"0\" maxOccurs=\"unbounded\">\n";
    // There doesn't seem to be any way of specifying the element type
    // here without also constraining the element name!
    //    os << "      <xs:element name=\".*\" type=\""<<xsd_typeName<T>()<<"\"/>\n";
    os << "      <xs:any processContents=\"lax\"/>\n";
    os << "    </xs:sequence>\n";
    os << "  </xs:complexType>\n";
    g.addMember(tail(d), xsd_typeName<StringKeyMap<T> >());
    g.defineType(type, os.str());
    g.addDependency(type, xsd_typeName<T>());
    // ensure that the value type as a definition also
    xsd_generate(g,"",T());
    
  }
}

using classdesc::xsd_generate;

namespace classdesc_access
{
  namespace cd=classdesc;

  template <class T>
  struct access_json_pack<classdesc::StringKeyMap<T> >
  {
    template <class U>
    void operator()(cd::json_pack_t& j,const cd::string& d,U& x)
    {json_pack_stringKeyMap(j,d,x);}
  };

  template <class T>
  struct access_json_unpack<classdesc::StringKeyMap<T> >
  {
    void operator()(cd::json_unpack_t& j,const cd::string& d,cd::StringKeyMap<T>& x)
    {json_unpack_stringKeyMap(j,d,x);}
    void operator()(cd::json_unpack_t& j,const cd::string& d,const cd::StringKeyMap<T>& x)
    {}
  };

  template <class T>
  struct access_xml_pack<classdesc::StringKeyMap<T> >
  {
    template <class U>
    void operator()(cd::xml_pack_t& x,const cd::string& d,U& arg)
    {
      cd::xml_pack_t::Tag tag(x,d); 
      for (typename cd::StringKeyMap<T>::const_iterator i=arg.begin(); 
           i!=arg.end(); ++i)
        ::xml_pack(x,d+"."+i->first,i->second);
    }
  };

  template <class T>
  struct access_xml_unpack<classdesc::StringKeyMap<T> >
  {
    template <class U>
    void operator()(cd::xml_unpack_t& x,const cd::string& d,U& arg)
    {
      for (cd::xml_unpack_t::ContentMap::const_iterator i=x.firstToken(d);
           i!=x.endToken(d); ++i)
        {
          std::string key=i->first;
          size_t p=i->first.rfind('.');
          if (p!=std::string::npos) key=key.substr(p);
          ::xml_unpack(x,i->first,arg[key]);
        }
    }
  };
  
}


#ifdef _CLASSDESC
#pragma omit json_pack classdesc::StringKeyMap
#pragma omit json_unpack classdesc::StringKeyMap
#pragma omit xml_pack classdesc::StringKeyMap
#pragma omit xml_unpack classdesc::StringKeyMap
#pragma omit xsd_generate classdesc::StringKeyMap
#endif
#include "stringKeyMap.cd"
#endif

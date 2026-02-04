/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_XML_PACK_EPILOGUE_H
#define CLASSDESC_XML_PACK_EPILOGUE_H
#include <typeName_epilogue.h>
#include "polyAccessXMLPack.h"
#include "xml_pack-allCDs.h"

namespace classdesc
{
  class xml_pack_t;
  class xml_unpack_t;
}

namespace classdesc_access
{
#ifdef CLASSDESC_XML_PACK_BASE_H
  template <class T> struct access_xml_pack
  {
    //This routine uses operator << if defined
    void operator()(classdesc::xml_pack_t& t,const classdesc::string& d, const T& a)
    {t.pack_notag(d,a);}
  };

#if defined(__cplusplus) && __cplusplus>=201703L
  
  template <class... A>
  struct access_xml_pack<std::tuple<A...>>
  {
    template <class T>
    void xml_pack_tuple_element(classdesc::xml_pack_t& o, const cd::string& d, T& a)
    {
      if constexpr (std::tuple_size<T>::value>0)
        {
          // element name is given by the type name
          std::string eName=classdesc::typeName<typename std::tuple_element<0,T>::type>();
          eName=eName.substr(0,eName.find('<')); //trim off any template args
          // strip leading namespace and qualifiers
          const char *e=eName.c_str()+eName.length();
          while (e!=eName.c_str() && *(e-1)!=' ' && *(e-1)!=':') e--;
          ::xml_pack(o,e,std::get<0>(a));
          auto tail=classdesc::tupleTail(a);
          xml_pack_tuple_element(o,d,tail);
        }
    }

    template <class U>
    void operator()(classdesc::xml_pack_t& o, const cd::string& d, U& a)
    {
      classdesc::xml_pack_t::Tag tag(o,d); 
      xml_pack_tuple_element(o, d, a);
    }
  };
#endif
  
#endif

#ifdef CLASSDESC_XML_UNPACK_BASE_H
  //This routine uses operator >> if defined
  template <class T> struct access_xml_unpack
  {
    void operator()(classdesc::xml_unpack_t& t,const classdesc::string& d,T& a)
    {t.unpack(d,a);}
  };
  // partial specialisation to discard unpacking to constant data types
  template <class T> struct access_xml_unpack<const T>
  {
    void operator()(classdesc::xml_unpack_t& t,const classdesc::string& d, const T& a)
    {
      T tmp;
      t.unpack(d,tmp);
    }
  };

#if defined(__cplusplus) && __cplusplus>=201703L
  template <class... A>
  struct access_xml_unpack<std::tuple<A...>>
  {
    template <class T>
    void xml_unpack_tuple_element(classdesc::xml_unpack_t& o, const cd::string& d, T& a, int i)
    {
      if constexpr (std::tuple_size<T>::value>0)
        {
          ::xml_unpack(o,classdesc::idx(d,i),std::get<0>(a));
          auto tail=classdesc::tupleTail(a);
          xml_unpack_tuple_element(o,d,tail,i+1);
          a=std::tuple_cat(std::make_tuple(std::get<0>(a)), tail);
        }
    }
    
    template <class U>
    void operator()(classdesc::xml_unpack_t& o, const cd::string& d, U& a)
    {
      // element name is given by the type name
      std::string eName=classdesc::typeName<typename std::tuple_element<0,std::tuple<A...>>::type>();
      eName=eName.substr(0,eName.find('<')); //trim off any template args
      // strip leading namespace and qualifiers
      const char *e=eName.c_str()+eName.length();
      while (e!=eName.c_str() && *(e-1)!=' ' && *(e-1)!=':') e--;
      xml_unpack_tuple_element(o,d+"."+e,a,0);
    }
  };
#endif

#endif
}

namespace classdesc
{
  template <class T>
  struct AllOtherXMLPackpTypes: public 
  Not< 
    Or<
      Or<
        Or< is_sequence<T>, is_associative_container<T> >, 
        is_fundamental<T> 
        >,
      is_enum<T>
      >
    >
  {};
}

namespace classdesc
{
#ifdef CLASSDESC_XML_PACK_BASE_H
  template <class T> typename
  enable_if<AllOtherXMLPackpTypes<T>, void >::T
  xml_packp(xml_pack_t& t,const string& d, T& a, dummy<0> dum=0)
  {
    xml_pack_t::Tag tag(t,d);
    classdesc_access::access_xml_pack<T>()(t,d,a);
  }
#endif  

#ifdef CLASSDESC_XML_UNPACK_BASE_H
 template <class T> typename
  enable_if<AllOtherXMLPackpTypes<T>, void >::T
  xml_unpackp(xml_unpack_t& t,const string& d,T& a, dummy<0> dum=0)
  {
    classdesc_access::access_xml_unpack<T>()(t,d,a);
  }

  template <class T> typename
  enable_if<is_enum<T>, void >::T
  xml_unpackp(xml_unpack_t& t,const string& d,T& a, dummy<1> dum=0)
  {
    classdesc_access::access_xml_unpack<T>()(t,d,a);
  }

  // handle const arguments
  template <class T> 
  void xml_unpackp(xml_unpack_t& t,const string& d, const T& a)
  {}
#endif

#ifdef CLASSDESC_XML_PACK_BASE_H
  template <class T> void xml_pack(xml_pack_t& t,const string& d, T& a)
  {
    xml_packp(t,d,a);
  }
  
  template <class T> void xml_pack(xml_pack_t& t,const string& d, const T& a)
  {
    xml_packp(t,d,const_cast<T&>(a));
  }

  template <class T> xml_pack_t& operator<<(xml_pack_t& t, const T& a)
  {xml_pack(t,"root",const_cast<T&>(a)); return t;}

#endif
  
#ifdef CLASSDESC_XML_UNPACK_BASE_H
  template <class T> void xml_unpack(xml_unpack_t& t,const string& d,T& a)
  {
    xml_unpackp(t,d,a);
  }
  
  template <class T> xml_unpack_t& operator>>(xml_unpack_t& t, T& a)
  {xml_unpack(t,"root",a); return t;}
#endif
}

namespace classdesc_access
{
  namespace cd=classdesc;

  template <> struct access_xml_pack<cd::CDATA>
  {
    void operator()(cd::xml_pack_t& p,const std::string& d,const cd::CDATA& c)
    {
      p.pack_notag(d,c);
    }
  };

  template <class T1, class T2>
  struct access_xml_pack<std::pair<T1,T2> >
  {
    template <class U>
    void operator()(cd::xml_pack_t& x, const cd::string& d, U& arg)
    {
      cd::xml_pack_t::Tag t(x,d);
      xml_pack(x,d+".first",arg.first);
      xml_pack(x,d+".second",arg.second);
    }
  };

  template <class T1, class T2>
  struct access_xml_unpack<std::pair<T1,T2> >
  {
    template <class U>
    void operator()(cd::xml_unpack_t& x, const cd::string& d, U& arg)
    {
      xml_unpack(x,d+".first",arg.first);
      xml_unpack(x,d+".second",arg.second);
    }
  };

#ifdef CLASSDESC_POLYPACKBASE_H
#ifdef CLASSDESC_XML_PACK_BASE_H
  template <> struct access_xml_pack<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::xml_pack_t> {};
  template <class T> struct access_xml_pack<cd::PolyPack<T> >: 
    public cd::NullDescriptor<cd::xml_pack_t> {};
#endif
#ifdef CLASSDESC_XML_UNPACK_BASE_H
   template <> struct access_xml_unpack<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::xml_unpack_t> {};
  template <class T> struct access_xml_unpack<cd::PolyPack<T> >: 
    public cd::NullDescriptor<cd::xml_unpack_t> {};
#endif
#endif

#ifdef CLASSDESC_POLYJSONBASE_H
#ifdef CLASSDESC_XML_PACK_BASE_H
  template <> struct access_xml_pack<cd::PolyJsonBase>: 
    public cd::NullDescriptor<cd::xml_pack_t> {};
  template <class T> struct access_xml_pack<cd::PolyJson<T> >: 
    public cd::NullDescriptor<cd::xml_pack_t> {};
#endif
#ifdef CLASSDESC_XML_UNPACK_BASE_H
  template <> struct access_xml_unpack<cd::PolyJsonBase>: 
    public cd::NullDescriptor<cd::xml_unpack_t> {};
  template <class T> struct access_xml_unpack<cd::PolyJson<T> >: 
    public cd::NullDescriptor<cd::xml_unpack_t> {};
#endif
#endif

#ifdef CLASSDESC_POLYXMLBASE_H
//#ifdef CLASSDESC_XML_PACK_BASE_H
//  template <> struct access_xml_pack<cd::PolyXMLBase>: 
//    public cd::NullDescriptor<cd::xml_pack_t> {};
//  template <class T> struct access_xml_pack<cd::PolyXML<T> >: 
//    public cd::NullDescriptor<cd::xml_pack_t> {};
//#endif
//#ifdef CLASSDESC_XML_UNPACK_BASE_H
//  template <> struct access_xml_unpack<cd::PolyXMLBase>: 
//    public cd::NullDescriptor<cd::xml_unpack_t> {};
//  template <class T> struct access_xml_unpack<cd::PolyXML<T> >: 
//    public cd::NullDescriptor<cd::xml_unpack_t> {};
//#endif
#endif

#ifdef CLASSDESC_REF_H
#ifdef CLASSDESC_XML_PACK_BASE_H
  template <class T> struct access_xml_pack<cd::ref<T> >
  {
    void operator()(cd::xml_pack_t& x, const cd::string& d, cd::ref<T>& a)
    {
      if (a) xml_pack(x,d,*a);
    }
  };
#endif

#ifdef CLASSDESC_XML_UNPACK_BASE_H
  template <class T> struct access_xml_unpack<cd::ref<T> >
  {
    void operator()(cd::xml_unpack_t& x, const cd::string& d, cd::ref<T>& a)
    {
      if (x.exists(d))
        xml_unpack(x,d,*a);
    }
  };
#endif
#endif

#ifdef CLASSDESC_XML_PACK_BASE_H
  template<class T>
  struct access_xml_pack<T*>
  {
    void operator()(cd::xml_pack_t& targ, const cd::string& desc, T*&)
    {
      throw cd::exception("xml_pack of pointers not supported");
    } 
  };
#endif

#ifdef CLASSDESC_XML_UNPACK_BASE_H
  template<class T>
  struct access_xml_unpack<T*>
  {
    void operator()(cd::xml_unpack_t& targ, const cd::string& desc, T*&)
    {
      throw cd::exception("xml_unpack of pointers not supported");
    } 
  };
#endif
}

#endif

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef POLY_ACCESS_XML_PACK_H
#define POLY_ACCESS_XML_PACK_H

namespace classdesc
{
#ifdef XML_PACK_BASE_H
#ifdef POLYXMLBASE_H
  // polymorphic version
  template <class T>
  typename enable_if<is_base_of<PolyXMLBase, typename T::element_type>, void>::T
  xml_pack_smart_ptr(xml_pack_t& x, const string& d, const T& a) 
  {
    if (a.get())
      {
        // requires a bit of jiggery-pokery to get the tag wrappers
        // happening in the right order
        xml_pack_t::Tag t(x,d);
        ::xml_pack(x,d+".type",a->type());
        a->xml_pack(x,d);
      }
  }

  // non polymorphic version
  template <class T>
  typename enable_if<Not<is_base_of<PolyXMLBase, typename T::element_type> >, void>::T
#else
  template <class T>
  void
#endif
  xml_pack_smart_ptr(xml_pack_t& x, const string& d, const T& a)
  {
    if (a)
      ::xml_pack(x,d,*a);
  }
  

  // special handling of shared pointers to avoid a double wrapping problem
  template<class T>
  void xml_pack(xml_pack_t& x, const string& d, shared_ptr<T>& a)
  {xml_pack_smart_ptr(x,d,a);}

#if defined(__cplusplus) && __cplusplus<=201402
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  template<class T>
  void xml_pack(xml_pack_t& x, const string& d, std::auto_ptr<T>& a)
  {xml_pack_smart_ptr(x,d,a);}
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
  template<class T, class D>
  void xml_pack(xml_pack_t& x, const string& d, std::unique_ptr<T,D>& a)
  {xml_pack_smart_ptr(x,d,a);}
#endif

#endif

#ifdef XML_UNPACK_BASE_H
#ifdef POLYXMLBASE_H
    // polymorphic version
  template <class T>
  typename enable_if<is_base_of<PolyXMLBase, typename T::element_type>, void>::T
  xml_unpack_smart_ptr(xml_unpack_t& x, const string& d, T& a, 
                        dummy<0> dum=0)
  {
    if (x.exists(d+".type"))
      {
        typename T::element_type::Type type;
        ::xml_unpack(x,d+".type",type);
        a.reset(T::element_type::create(type));
        a->xml_unpack(x,d);
      }
    else
      a.reset();
  }

  // non polymorphic version
  template <class T>
  typename enable_if<Not<is_base_of<PolyXMLBase, typename T::element_type> >, void>::T
#else
  template <class T>
  void
#endif
  xml_unpack_smart_ptr(xml_unpack_t& x, const string& d, 
                        T& a, dummy<1> dum=0)
  {
    if (x.exists(d))
      {
        a.reset(new typename T::element_type);
        ::xml_unpack(x,d,*a);
      }
    else
      a.reset();
  }

  // const argument versions of above
#ifdef POLYXMLBASE_H
    // polymorphic version
  template <class T>
  typename enable_if<is_base_of<PolyXMLBase, typename T::element_type>, void>::T
  xml_unpack_smart_ptr(xml_unpack_t& x, const string& d, const T& a, 
                        dummy<0> dum=0)
  {
    if (x.exists(d+".type"))
      {
        typename T::element_type::Type type;
        ::xml_unpack(x,d+".type",type);
        T tmp(T::element_type::create(type));
        tmp->xml_unpack(x,d);
      }
  }

    // non polymorphic version
  template <class T>
  typename enable_if<Not<is_base_of<PolyXMLBase, typename T::element_type> >, void>::T
#else
  template <class T>
  void
#endif
  xml_unpack_smart_ptr(xml_unpack_t& x, const string& d, 
                        const T& a, dummy<1> dum=0)
  {
    if (x.exists(d))
      {
        T tmp(new typename T::element_type);
        ::xml_unpack(x,d,*tmp);
      }
  }
#endif
}

namespace classdesc_access
{
  namespace cd = classdesc;

#ifdef XML_PACK_BASE_H
  template <class T>
  struct access_xml_pack<cd::shared_ptr<T> >
  {
    void operator()(cd::xml_pack_t& x, const cd::string& d, const cd::shared_ptr<T>& a)
    {xml_pack_smart_ptr(x,d,a);}
  };

#if defined(__cplusplus) && __cplusplus<=201402
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  template <class T>
  struct access_xml_pack<std::auto_ptr<T> >
  {
    void operator()(cd::xml_pack_t& x, const cd::string& d, const std::auto_ptr<T>& a)
    {xml_pack_smart_ptr(x,d,a);}
  };
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
  template <class T>
  struct access_xml_pack<std::unique_ptr<T> >
  {
    void operator()(cd::xml_pack_t& x, const cd::string& d, const std::unique_ptr<T>& a)
    {xml_pack_smart_ptr(x,d,a);}
  };
#endif
#endif
  
#ifdef XML_UNPACK_BASE_H
  template <class T>
  struct access_xml_unpack<cd::shared_ptr<T> >
  {
    template <class U>
    void operator()(cd::xml_unpack_t& x, const cd::string& d, U& a)
    {xml_unpack_smart_ptr(x,d,a);}
  };

#if defined(__cplusplus) && __cplusplus<=201402
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  template <class T>
  struct access_xml_unpack<std::auto_ptr<T> >
  {
    template <class U>
    void operator()(cd::xml_unpack_t& x, const cd::string& d, U& a)
    {xml_unpack_smart_ptr(x,d,a);}
  };
#if defined(__GNUC__) && !defined(__ICC) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
  template <class T>
  struct access_xml_unpack<std::unique_ptr<T> >
  {
    template <class U>
    void operator()(cd::xml_unpack_t& x, const cd::string& d, U& a)
    {xml_unpack_smart_ptr(x,d,a);}
  };
#endif
#endif
}
 

#endif

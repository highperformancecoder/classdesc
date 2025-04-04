/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef CLASSDESC_JSON_PACK_EPILOGUE_H
#define CLASSDESC_JSON_PACK_EPILOGUE_H
#include "classdesc.h"
#include <sstream>
#include "json_pack-allCDs.h"

namespace classdesc
{
  template <>
  struct tn<json_pack_t>
  {
    static string name() {return "classdesc::json_pack_t";}
  };
  
  template <class T>
  struct AllOtherJsonPackpTypes:
    public Not< Or< Or< Or< Or<is_fundamental<T>,is_string<T> >, is_sequence<T> >, 
                        And<is_associative_container<T>,Not<is_stringKeyMap<T> > > >, is_pointer<T> > >
  {};

  template <class T> typename 
  enable_if< AllOtherJsonPackpTypes<T>, void >::T
  json_packp(json_pack_t& o, const string& d, T& a)
  {
    o.objectify();
    if (tail(d)!="")
      {
        //create the object, if it doesn't already exist
        try
          {
            json5_parser::mValue& parent=json_find(o,head(d));
            if (parent.type()!=json5_parser::obj_type)
              throw json_pack_error("trying to create object %s in non-object",
                                    d.c_str());
            json5_parser::mObject::iterator member=parent.get_obj().find(tail(d));
            if (member==parent.get_obj().end())
              parent.get_obj().insert(make_pair(tail(d), json5_parser::mObject()));
          }
         catch (json_pack_error&)
          {
            // only throw if this flag is set
            if (o.throw_on_error) throw; 
          }
      }
    classdesc_access::access_json_pack<T>()(o,d,a);
  }

  template <class T> typename
  enable_if< AllOtherJsonPackpTypes<T>, void >::T
  json_unpackp(json_pack_t& o, const string& d, T& a, dummy<3> dum=0)
  {classdesc_access::access_json_unpack<T>()(o,d,a);}

  template <class T> void json_pack(json_pack_t& o, const string& d, T& a)
  {json_packp(o,d,a);}

  template <class T> void json_unpack(json_unpack_t& o, const string& d, T& a)
  {json_unpackp(o,d,a);}

  template <class T> void json_packp(json_pack_t& o, const string& d, T* a)
  {}
  
  template <class T> void json_packp(json_pack_t& o, const string& d, const char* a)
  {o=json_pack_t(a);}
}

namespace classdesc_access
{
  namespace cd=classdesc;

#if defined(__cplusplus) && __cplusplus>=201103L  
  // nobble iterators
  template <class T>
  struct access_json_pack<T, cd::void_t<typename std::iterator_traits<T>::value_type>>:
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <class T>
  struct access_json_unpack<T, cd::void_t<typename std::iterator_traits<T>::value_type>>:
    public cd::NullDescriptor<cd::json_unpack_t> {};
#endif
  
#ifndef JSON_PACK_NO_FALL_THROUGH_TO_STREAMING
  // fall through to streaming operators
  template <class T,class Enable>
  struct access_json_pack
  {
  public:
    void operator()(cd::json_pack_t& b, const cd::string& d, T& a)
    {
      std::ostringstream o;
      o<<a;
      b<<o.str();
    }
  };

  template <class T,class Enable>
  struct access_json_unpack
  {
  public:
    void operator()(cd::json_unpack_t& b, const cd::string& d, 
                    T& a)
    {
      std::string s;
      b>>s;
      std::istringstream i(s);
      i>>a;
    }
  };
#endif
  
#if defined(__cplusplus) && __cplusplus>=201103L 
  template <class T>
  struct access_json_pack<std::function<T>>
  {
  public:
    void operator()(cd::json_unpack_t& b, const cd::string& d, 
                    std::function<T>& a)
    {b<<cd::typeName<T>();}
  };
  template <class T>
  struct access_json_unpack<std::function<T>>
  {
  public:
    void operator()(cd::json_unpack_t& b, const cd::string& d, 
                    std::function<T>& a)
    {}
  };
#endif

  template <> struct access_json_pack<cd::json_pack_t>
  {
    template <class U>
    void operator()(cd::json_pack_t& b, const cd::string& d, U& a)
    {b=a;}
  };
  template <> struct access_json_unpack<cd::json_pack_t>
  {
    void operator()(cd::json_pack_t& b, const cd::string& d, cd::json_pack_t& a){a=b;}
    void operator()(cd::json_pack_t& b, const cd::string& d, const cd::json_pack_t& a){}
  };

  template <> struct access_json_pack<json5_parser::mValue>
  {
    template <class U>
    void operator()(cd::json_pack_t& b, const cd::string& d, U& a)
    {b=cd::json_pack_t(a);}
  };
  template <> struct access_json_unpack<json5_parser::mValue>
  {
    void operator()(cd::json_pack_t& b, const cd::string& d, json5_parser::mValue& a){a=b;}
    void operator()(cd::json_pack_t& b, const cd::string& d, const json5_parser::mValue& a){}
  };

  
  
  // support for polymorphic types, if loaded
//#ifdef NEW_POLY_H
//  template <class T> struct access_json_pack<cd::PolyBase<T> >: 
//    public cd::NullDescriptor<cd::json_pack_t> {};
//  template <class T> struct access_json_unpack<cd::PolyBase<T> >: 
//    public cd::NullDescriptor<cd::json_unpack_t> {};
//  template <class T, class B> struct access_json_pack<cd::Poly<T,B> >
//  {
//    template <class U>
//    void operator()(cd::json_pack_t& t, const cd::string& d, U& a)
//    {
//      json_pack(t,d,cd::base_cast<B>::cast(a));
//    }
//  };
//  template <class T, class B> struct access_json_unpack<cd::Poly<T,B> > 
//  {
//    template <class U>
//    void operator()(cd::json_pack_t& t, const cd::string& d, U& a)
//    {
//      json_unpack(t,d,cd::base_cast<B>::cast(a));
//    }
//  };
//#endif

#ifdef CLASSDESC_POLYPACKBASE_H
  template <> struct access_json_pack<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <> struct access_json_unpack<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
  template <class T> struct access_json_pack<cd::PolyPack<T> >: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <class T> struct access_json_unpack<cd::PolyPack<T> >: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
#endif

#ifdef CLASSDESC_POLYJSONBASE_H
  template <> struct access_json_pack<cd::PolyJsonBase>: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <> struct access_json_unpack<cd::PolyJsonBase>: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
  template <class T> struct access_json_pack<cd::PolyJson<T> >: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <class T> struct access_json_unpack<cd::PolyJson<T> >: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
#endif

#ifdef CLASSDESC_POLYXMLBASE_H
  template <> struct access_json_pack<cd::PolyXMLBase>: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <> struct access_json_unpack<cd::PolyXMLBase>: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
  template <class T> struct access_json_pack<cd::PolyXML<T> >: 
    public cd::NullDescriptor<cd::json_pack_t> {};
  template <class T> struct access_json_unpack<cd::PolyXML<T> >: 
    public cd::NullDescriptor<cd::json_unpack_t> {};
#endif

template <> struct access_json_pack< enum ::classdesc::RESTProcessType::Type > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::json_pack_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
::json_pack(targ,desc+"",(int&)arg);
}
template <class _CD_TYPE>
void type(classdesc::json_pack_t& targ, const classdesc::string& desc)
{
}
};
template <> struct access_json_pack< struct ::classdesc::RESTProcessType > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::json_pack_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
}
template <class _CD_TYPE>
void type(classdesc::json_pack_t& targ, const classdesc::string& desc)
{
}
};

template <> struct access_json_unpack< enum ::classdesc::RESTProcessType::Type > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::json_unpack_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
::json_unpack(targ,desc+"",(int&)arg);
}
template <class _CD_TYPE>
void type(classdesc::json_unpack_t& targ, const classdesc::string& desc)
{
}
};
template <> struct access_json_unpack< struct ::classdesc::RESTProcessType > {
template <class _CD_ARG_TYPE>
void operator()(classdesc::json_unpack_t& targ, const classdesc::string& desc,_CD_ARG_TYPE& arg)
{
}
template <class _CD_TYPE>
void type(classdesc::json_unpack_t& targ, const classdesc::string& desc)
{
}
};
}  

#include "polyAccessJsonPack.h"

#endif

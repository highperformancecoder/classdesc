/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef JSON_PACK_BASE_H
#define JSON_PACK_BASE_H
#include "classdesc.h"
#define JSON5_PARSER_MVALUE_ENABLED
#include <json5_parser_value.h>
#include <json5_parser_reader_template.h>
#include <json5_parser_writer_template.h>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <map>

namespace classdesc
{
  
  class json_pack_error : public exception 
  {
    static const int maxchars=200; /* I hope this will always be large enough */
    char errstring[maxchars];
  public:
    json_pack_error(const char *fmt,...)
     {
       va_list args;
       va_start(args, fmt);
       vsnprintf(errstring,maxchars,fmt,args);
       va_end(args);
     }
    virtual ~json_pack_error() throw() {}
    virtual const char* what() const throw() {return errstring;}
  };

  struct json_object_not_found: public json_pack_error
  {
    json_object_not_found(const string& name): 
      json_pack_error("json object %s not found", name.c_str()) {}
  };

  const inline std::map<json5_parser::Value_type,RESTProcessType::Type>& RESTProcessTypeJSONMap()
  {
    static std::map<json5_parser::Value_type,RESTProcessType::Type> jsonMap;
    if (jsonMap.empty())
      {
        jsonMap[json5_parser::obj_type]=RESTProcessType::object;
        jsonMap[json5_parser::array_type]=RESTProcessType::array;
        jsonMap[json5_parser::str_type]=RESTProcessType::string;
        jsonMap[json5_parser::bool_type]=RESTProcessType::boolean;
        jsonMap[json5_parser::int_type]=RESTProcessType::int_number;
        jsonMap[json5_parser::real_type]=RESTProcessType::float_number;
        jsonMap[json5_parser::null_type]=RESTProcessType::null;
      }
    return jsonMap;
  };
  
  // these are classes, not typedefs to avoid adding properties to mValue
  class json_pack_t: public json5_parser::mValue
  {
  public:
    bool throw_on_error; ///< enable exceptions on error conditions
    bool throw_on_not_found; ///< enable exceptions if element not present in JSON stream

    typedef json5_parser::mArray Array;
    json_pack_t(): json5_parser::mValue(json5_parser::mObject()), 
                   throw_on_error(false), throw_on_not_found(false)  {}
    
    template <class T>
    explicit json_pack_t(const T& x,
                         typename enable_if<is_base_of<json5_parser::mValue,T>, dummy<0> >::T* d=0 ): 
      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}

    template <class T>
    explicit json_pack_t(const T& x,
                         typename enable_if<is_base_of<json5_parser::mArray,T>, dummy<0> >::T* d=0 ): 
      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}

    template <class T> 
    explicit json_pack_t(const T& x, typename enable_if<And<Not<is_base_of<json5_parser::mValue,T> >,Not<is_base_of<json5_parser::mArray,T> > >, dummy<1> >::T* d=0);

    explicit json_pack_t(bool x):
      json5_parser::mValue(x),
      throw_on_error(false), throw_on_not_found(false) {}
    explicit json_pack_t(double x):
      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}
    explicit json_pack_t(const char* x):
      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}
    explicit json_pack_t(const string& x):
      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}
//    explicit json_pack_t(const Array& x):
//      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}
//    explicit json_pack_t(const json5_parser::mValue& x):
//      json5_parser::mValue(x), throw_on_error(false), throw_on_not_found(false) {}
#if defined(__cplusplus) && __cplusplus>=201103L
    template <class T>
    explicit json_pack_t(const std::initializer_list<T>& x): json5_parser::mValue(json5_parser::mArray())
    {
      auto& arr=get_array();
      for (auto& i: x) arr.emplace_back(i);
    }
#endif

    const Array& array() const {return get_array();}
    RESTProcessType::Type type() const {
      return RESTProcessTypeJSONMap().find(json5_parser::mValue::type())->second;
    }
  };

  inline bool read(const std::string& s, json_pack_t& value)
  {
    return json5_parser::read_string(s, static_cast<json5_parser::mValue&>(value));
  }
  
  inline bool read(std::istream& is, json_pack_t& value)
  {
    return json5_parser::read_stream(is, static_cast<json5_parser::mValue&>(value));
  }
     
  inline void write(const json_pack_t& value, std::ostream& os, unsigned options=0)
  {
    json5_parser::write_stream(static_cast<const json5_parser::mValue&>(value), os, options );
  }

  inline std::string write(const json_pack_t& value, unsigned options=0)
  {
    return json5_parser::write_string(static_cast<const json5_parser::mValue&>(value), options );
  }
  
  inline void write_formatted(const json_pack_t& value, std::ostream& os)
  {
    write_stream( static_cast<const json5_parser::mValue&>(value), os, json5_parser::pretty_print );
  }

  inline std::string write_formatted(const json_pack_t& value)
  {
    return write_string( static_cast<const json5_parser::mValue&>(value), json5_parser::pretty_print );
  }

  typedef json_pack_t json_unpack_t;

  /// forward declare generic json operations
  template <class T> void json_pack(json_pack_t& o, const string& d, T& a);

  template <class T> void json_pack(json_pack_t& o, const string& d, const T& a)
  {json_pack(o,d,const_cast<T&>(a));}
 
  template <class T> void json_unpack(json_unpack_t& o, const string& d, T& a);

  template <class T> json_pack_t& operator<<(json_pack_t& j, const T& a) 
  {json_pack(j,"",a); return j;}

  template <class T> const json_unpack_t& operator>>(const json_unpack_t& j, T& a) 
  {json_unpack(const_cast<json_unpack_t&>(j),"",a); return j;}
  
  inline const json_unpack_t& operator>>(const json_unpack_t& j, const char*& a) 
  {throw json_pack_error("cannot unpack to char*, please use string instead");}

  template <class T>
  json_pack_t::json_pack_t(const T& x, typename enable_if<And<Not<is_base_of<json5_parser::mValue,T> >,Not<is_base_of<json5_parser::mArray,T> > >, dummy<1> >::T*):
    json5_parser::mValue(json5_parser::mObject()),
    throw_on_error(false), throw_on_not_found(false)
  {(*this)<<x;}
  
  /// find an object named by \a name within the json object x
  inline json5_parser::mValue& 
  json_find(json5_parser::mValue& x, std::string name)
  {
    if (name.size()==0) return x;
    if (name[0]=='.') name.erase(0,1); //remove leading '.'
    std::string::size_type p=name.find('.');
    if (x.type()==json5_parser::obj_type)
      {
        json5_parser::mObject& xo=x.get_obj();
        json5_parser::mObject::iterator i=xo.find(name.substr(0,p));
        if (i==xo.end())
          throw json_object_not_found(name.substr(0,p));
        else if (p==std::string::npos)
          return i->second;
        else
          return json_find(i->second,name.substr(p,std::string::npos));
      }
    else
      throw json_pack_error("%s is not a json object",name.c_str());
  }

  //json5_parser::mValue does not provide constructors for everything. Oh well..
  template <class T> json5_parser::mValue valueof(T a) 
  {return json5_parser::mValue(a);}
  template <class T> T getValue(const json5_parser::mValue& x) 
  {return x.get_value<T>();} 

  inline json5_parser::mValue valueof(unsigned char a)  
  {return json5_parser::mValue(int(a));}
  template <> inline unsigned char getValue(const json5_parser::mValue& x) 
  {return x.get_value<int>();}

  inline json5_parser::mValue valueof(signed char a)  
  {return json5_parser::mValue(int(a));}
  template <> inline signed char getValue(const json5_parser::mValue& x) 
  {return x.get_value<int>();}

  inline json5_parser::mValue valueof(char a)  
  {return json5_parser::mValue(string()+a);}
  template <> inline char getValue(const json5_parser::mValue& x) 
  {return x.get_value<string>()[0];}

  inline json5_parser::mValue valueof(unsigned short a)  
  {return json5_parser::mValue(int(a));}
  template <> inline unsigned short getValue(const json5_parser::mValue& x) 
  {return x.get_value<int>();}

  inline json5_parser::mValue valueof(signed short a)  
  {return json5_parser::mValue(int(a));}
  template <> inline signed short getValue(const json5_parser::mValue& x) 
  {return x.get_value<int>();}

  inline json5_parser::mValue valueof(unsigned int a)  
  {return json5_parser::mValue(boost::uint64_t(a));}
  template <> inline unsigned getValue(const json5_parser::mValue& x) 
  {return x.get_value<boost::uint64_t>();}

  inline json5_parser::mValue valueof(unsigned long a)  
  {return json5_parser::mValue(boost::uint64_t(a));}
  template <> inline unsigned long getValue(const json5_parser::mValue& x) 
  {return x.get_value<boost::uint64_t>();}

  inline json5_parser::mValue valueof(long a)  
  {return json5_parser::mValue(boost::int64_t(a));}
  template <> inline long getValue(const json5_parser::mValue& x) 
  {return x.get_value<boost::int64_t>();}

#ifdef HAVE_LONGLONG
  inline json5_parser::mValue valueof(unsigned long long a)  
  {return json5_parser::mValue(boost::uint64_t(a));}
  template <> inline unsigned long long getValue(const json5_parser::mValue& x) 
  {return x.get_value<boost::uint64_t>();}

  inline json5_parser::mValue valueof(long long a)  
  {return json5_parser::mValue(boost::int64_t(a));}
  template <> inline long long getValue(const json5_parser::mValue& x) 
  {return x.get_value<boost::int64_t>();}
#endif

  inline json5_parser::mValue valueof(float a)  
  {return json5_parser::mValue(double(a));}
  template <> inline float getValue(const json5_parser::mValue& x) 
  {return x.get_value<double>();}

 // basic types
  template <class T> typename
  enable_if<Or<is_fundamental<T>,is_string<T> >, void>::T
  json_packp(json_unpack_t& o, const string& d, const T& a, dummy<0> dum=0)
  {
    using namespace json5_parser;
    if (d=="")
      o=json_unpack_t(valueof(a));
    else
      {
        try
          {
            json5_parser::mValue& parent=json_find(o,head(d));
            if (parent.type()==obj_type)
              parent.get_obj()[tail(d)]=valueof(a);
            else
              throw json_pack_error("cannot add to a basic type");
          }
        catch (const json_object_not_found&)
          {
            // only throw if this flag is set
            if (o.throw_on_not_found) throw; 
          }
        catch (const json_pack_error&)
          {
            // only throw if this flag is set
            if (o.throw_on_error) throw; 
          }
      }
   } 

  // basic types
  template <class T> typename
  enable_if<Or<is_fundamental<T>,is_string<T> >, void>::T
  json_unpackp(json_unpack_t& o, string d, T& a, dummy<0> dum=0)
  {
    try
      {
        a=getValue<T>(json_find(o,d));
      }
    catch (const json_pack_error&)
      {
        // only throw if this flag is set
        if (o.throw_on_error) throw; 
      }
  }  

  template <class T> void json_pack_isarray
  (json5_parser::mValue& jval, const T& val, std::vector<size_t> dims) 
  {
    if (dims.empty())
      {
        json_pack_t j;
        json_pack(j,"",val);
        jval=j;
      }
    else
      {
        size_t s=dims.back();
        jval=json5_parser::mArray(s);
        dims.pop_back();
        size_t stride=1;
        for (size_t i=0; i<dims.size(); ++i) stride*=dims[i];
        for (size_t i=0; i<s; ++i)
          json_pack_isarray(jval.get_array()[i],(&val)[i*stride], dims);
      }
  }
                                            
  // array handling
  template <class T>
  void json_pack(json_pack_t& o, const string& d, is_array ia, const T& a, 
            int ndims,size_t ncopies,...)
  {
  va_list ap;
  va_start(ap,ncopies);
  std::vector<size_t> dims(ndims);
  dims[ndims-1]=ncopies;
  for (int i=ndims-2; i>=0; --i) dims[i]=va_arg(ap,size_t);
  va_end(ap);
  try
    {
      json5_parser::mValue& parent=json_find(o,head(d));
      if (parent.type()!=json5_parser::obj_type)
        throw json_pack_error("attempt to pack an array member into a non-object");
      else
        json_pack_isarray(parent.get_obj()[tail(d)],a,dims);
    }
    catch (json_pack_error&)
      {
        // only throw if this flag is set
        if (o.throw_on_error) throw; 
      }
  }
 
  template <class T> void json_unpack_isarray
  (const json5_parser::mValue& jval, T& val, std::vector<size_t> dims) 
  {
    if (dims.empty())
      {
        json_unpack_t j(jval);
        json_unpack(j,"",val);
      }
    else
      {
        size_t s=dims.back();
        dims.pop_back();
        size_t stride=1;
        for (size_t i=0; i<dims.size(); ++i) stride*=dims[i];
        for (size_t i=0; i<s; ++i)
          json_unpack_isarray(jval.get_array()[i],(&val)[i*stride], dims);
      }
  }
                                            
  template <class T>
  void json_unpack(json_unpack_t& o, const string& d, is_array ia, T& a, 
            int ndims,size_t ncopies,...)
  {
    va_list ap;
    va_start(ap,ncopies);
    std::vector<size_t> dims(ndims);
    dims[ndims-1]=ncopies;
    for (int i=ndims-2; i>=0; --i) dims[i]=va_arg(ap,size_t);
    va_end(ap);
    try
      {
        const json5_parser::mValue& v=json_find(o,d);
        if (v.type()!=json5_parser::array_type)
          throw json_pack_error
            ("attempt to unpack an array member from a non-object");
        else 
          json_unpack_isarray(v,a,dims);
      }
    catch (json_pack_error&)
      {
        // only throw if this flag is set
        if (o.throw_on_error) throw; 
      }
    
  }

    
  /**
     handle enums
  */

  template <class T> void json_pack(json_pack_t& x, const string& d,
                                    Enum_handle<T> arg)
  {
    string tmp(static_cast<string>(arg));
    json_pack(x,d,tmp);
  }

  //Enum_handles have reference semantics
  template <class T> void json_unpack(json_unpack_t& x, const string& d,
                                    Enum_handle<T> arg)
  {
    std::string tmp;
    json_unpack(x,d,tmp);
    // remove extraneous white space
    int (*isspace)(int)=std::isspace;
    std::string::iterator end=std::remove_if(tmp.begin(),tmp.end(),isspace);
    arg=tmp.substr(0,end-tmp.begin());
  }

  /** standard container handling */
  template <class T> typename
  enable_if<is_sequence<T>, void>::T
  json_unpackp(json_unpack_t& o, const string& d, T& a, dummy<1> dum=0)
  {
    try
      {
        const json5_parser::mValue& val=json_find(o,d);
        if (val.type()!=json5_parser::array_type)
          throw json_pack_error("%s is not an array",d.c_str());
        else
          {
            const json5_parser::mArray& arr=val.get_array();
            resize(a, arr.size());
            size_t i=0;
            for (typename T::iterator j=a.begin(); i<arr.size() && j!=a.end(); ++i, ++j)
              {
                json_unpack_t jp(arr[i]);
                json_unpack(jp,"",*j);
              }
          }
      }
    catch (json_pack_error&)
      {
        if (o.throw_on_error) throw;
      }
  }

  template <class T1, class T2> 
  void json_pack(json_pack_t& o, const string& d, std::pair<T1,T2>& a)
  {
    json_pack(o,d+".first",a.first);
    json_pack(o,d+".second",a.second);
  }

  template <class T1, class T2>
  void json_unpackp(json_unpack_t& o, const string& d, std::pair<T1,T2>& a)
  {
    json_unpack(o,d+".first",a.first);
    json_unpack(o,d+".second",a.second);
  }

  template <class T> typename
  enable_if<Or<is_sequence<T>,is_associative_container<T> >, void>::T
  json_packp(json_pack_t& o, const string& d, const T& a, dummy<1> dum=0)
  {
  try
    {
      json5_parser::mValue& parent=json_find(o,head(d));
      if (parent.type()!=json5_parser::obj_type)
        throw json_pack_error("attempt to pack an array member into a non-object");
      else
        {
          json5_parser::mValue* v;
          if (d.empty())
            v=&parent;
          else
            v=&parent.get_obj()[tail(d)];

          json5_parser::mArray& arr=
            (*v=json5_parser::mArray(a.size())).get_array();
          typename T::const_iterator i=a.begin();
          for (size_t k=0; i!=a.end(); ++i, ++k)
            {
              json_pack_t j;
              json_pack(j,"",*i);
              arr[k]=j;
            }
        }
    }
  catch (json_pack_error&)
    {
      if (o.throw_on_error) throw;
    }
  }

  template <class T> typename
  enable_if<And<is_associative_container<T>, Not<is_stringKeyMap<T> > >, void>::T
  json_unpackp(json_unpack_t& o, const string& d, T& a, dummy<2> dum=0)
  {
    try
      {
        const json5_parser::mValue& val=json_find(o,d);
        if (val.type()!=json5_parser::array_type)
          throw json_pack_error("%s is not an array",d.c_str());
        else
          {
            const json5_parser::mArray& arr=val.get_array();
            a.clear();
            for (size_t i=0; i<arr.size(); ++i)
              {
                typename NonConstKeyValueType<typename T::value_type>::T v;
                json_unpack_t j(arr[i]);
                json_unpack(j,"",v);
                a.insert(v);
              }
          }
      }
    catch (json_pack_error&)
      {
        if (o.throw_on_error) throw;
      }
  }

  /*
    const static support
  */
  template <class T>
  void//typename enable_if<Not<is_pointer<T> >,void>::T
  json_pack(json_pack_t& targ, const string& desc, is_const_static, T) 
  {}

  template <class T>
  void//typename enable_if<Not<is_pointer<T> >,void>::T
  json_unpack(json_unpack_t& targ, const string& desc, is_const_static, T) 
  {}

  template <class T>
  typename enable_if<is_object<T>, void>::T
  json_unpack(json_unpack_t& targ, const string& desc, T*) {}
  
  template <class T>
  void json_unpack(json_unpack_t& targ, const string& desc, const T& arg) {}

  template <class T>
  void json_pack(json_pack_t& targ, const string& desc, Exclude<T>& arg) {}

  template <class T>
  void json_unpack(json_unpack_t& targ, const string& desc, Exclude<T>& arg) {} 

  template <class T, class U>
  void json_pack(json_pack_t& targ, const string& desc, T&, is_constructor, U&) {}

  template <class T, class U>
  void json_unpack(json_unpack_t& targ, const string& desc, T&, is_constructor, U&) {} 

  /// produce json string equivalent of object \a x
  template <class T> string json(const T& x) 
  {
    json_pack_t j;
    json_pack(j,"",x);
    return write(j);
  }
  template <class T> void json(T& x, const string& s) 
  {
    json_pack_t j;
    read(s, j);
    json_unpack(j,"",x);
  }

  template <class T>
  void json_pack_onbase(json_pack_t& x,const string& d,T& a)
  {json_pack(x,d+basename<T>(),a);}

  template <class T>
  void json_unpack_onbase(json_unpack_t& x,const string& d,T& a)
  {json_unpack(x,d+basename<T>(),a);}

}

inline std::ostream& operator<<(std::ostream& o, const json5_parser::mValue& x)
{
  json5_parser::write_stream(x,o);
  return o;
}

namespace classdesc_access
{
  template <class T> struct access_json_pack;
  template <class T> struct access_json_unpack;
}

#include "use_mbr_pointers.h"
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(json_pack)
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(json_unpack)
CLASSDESC_FUNCTION_NOP(json_pack)
CLASSDESC_FUNCTION_NOP(json_unpack)


using classdesc::json_pack_onbase;
using classdesc::json_unpack_onbase;

using classdesc::json_pack;
using classdesc::json_unpack;
#endif

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/**\file
\brief XML serialisation descriptor
*/

#ifndef XML_PACK_BASE_H
#define XML_PACK_BASE_H
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <assert.h>
#include <stdarg.h>

#include <classdesc.h>
#include <xml_common.h>
#include <stdexcept>

namespace classdesc
{
  inline std::string xml_quote(char c)
  {
    switch (c)
      {
      case '&': return "&amp;";
      case '<': return "&lt;";
      case '>': return "&gt;";
      case '\'': return "&apos;";
      case '"': return "&quot;";
      }
    if (!isgraph(c))
      {
        std::ostringstream s;
        s<<"&#"<<int(c)<<";";
        return s.str();
      }
    return std::string(1,c);
  }

  inline std::ostream& operator<<(std::ostream& o, const CDATA& x)
  {return o<<"<![CDATA["<<static_cast<const std::string&>(x)<<"]]>";}
  
  /**
     XML serialisation object
  */
  class xml_pack_t
  {
    std::ostream* o; // weak reference, allows for assignability 
    int taglevel;
    // count number of ids separated by '.'s in a string
    int level(const string& xx) {
      const char* x=xx.c_str();
      int l;
      if (*x=='\0') return 0;
      for (l=1; *x!='\0'; x++) if (*x=='.') l++;
      return l;
    }

    void pretty(const string& d) {if (prettyPrint) *o << std::setw(level(d)) <<"";}
    void endpretty() {if (prettyPrint) *o<<std::endl;}

    /**
       emit a tag \a d if current nesting level allows
       return true if tag created
    */
    bool tag(const string& d) {  
      int l=level(d);
      bool ret = taglevel < l; //return true if tag created
      if (ret)
        {
          pretty(d);
          *o<<"<"<<tail(d);
          if (l==1 && !schema.empty())
            *o<<" xmlns=\""<<schema<<"\"";
          *o<<">";
          endpretty();
          taglevel=l;
        }
      assert(taglevel==level(d));
      return ret;
    }
    /** emit end tag */
    void endtag(const string& d) {
      taglevel--;
      pretty(d);
      *o<<"</"<<tail(d)<<">";
      endpretty();
    }

    friend class Tag;
  public:
    string schema; 
    bool prettyPrint; /// if true, the layout XML in more human friendly form
    volatile bool abort=false; /// set to true to cancel packing from another thread
    struct PackAborted: public std::exception {};
    
    xml_pack_t(std::ostream& o, const string& schema=""): 
      o(&o), taglevel(0), schema(schema), prettyPrint(false) {}

    class Tag  ///<utility structure for handling tag/endtag
    {
      xml_pack_t* t;
      string d;
    public:
      Tag(xml_pack_t& t, const string& d): t(t.tag(d)? &t: 0), d(d) {}
      ~Tag() {if (t) t->endtag(d);}
    };

    // handle peculiar case sensitivity of floating point special values in XML
    template <class T>
    typename enable_if<is_floating_point<T>, std::ostream&>::T
    put(std::ostream& o, T x)
    {
      if (std::isnan(x))
        return o<<"NaN";
      else if (std::isinf(x))
        if (x<0)
          return o<<"-INF";
        else
          return o<<"INF";
      else
        return o<<x;
    }
        
    template <class T>
    typename enable_if<Not<is_floating_point<T> >, std::ostream&>::T
    put(std::ostream& o, const T& x)
    {
      return o<<x;
    }

    
    /**
       pack simple type
    */
    template <class T>
    void pack(const string& d, const T&x)
    {
      if (abort) throw PackAborted();
      std::string tag=tail(d);
      pretty(d);
      *o << "<"<<tag<<">";
      put(*o,x) << "</"<<tag<<">";
      endpretty();
      if (!*o) throw std::runtime_error("failed to serialise");
    }

    /**
       pack an untagged simple type
    */
    template <class T>
    void pack_notag(const string& d, const T&x) {
      if (abort) throw PackAborted();
      *o<<x;
      if (!*o) throw std::runtime_error("failed to serialise");
    }

  };

  template <class T>
  typename enable_if<is_fundamental<T>, void>::T
  xml_packp(xml_pack_t& x,const string& d,T& a) 
  {x.pack(d,a);}

  template <> inline void xml_packp(xml_pack_t& x,const string& d, bool& a)
  {x.pack(d, a? "true": "false");}

  template <> inline void xml_packp(xml_pack_t& x,const string& d, char& a)
  {x.pack(d,classdesc::xml_quote(a));}

  /**
     handle enums
  */

  template <class T> 
  typename enable_if<is_enum<T>, void>::T
  xml_packp(xml_pack_t& x, const string& d, T& arg)
  {x.pack(d,string(Enum_handle<T>(arg)));}



  template <class T> void xml_pack(xml_pack_t&,const string&, const T&);

  template <class T> void xml_pack(xml_pack_t&,const string&, T&);

  template <class T> xml_pack_t& operator<<(xml_pack_t& t, const T& a);

  inline void xml_pack(xml_pack_t& x,const string& d, std::string& a) 
  {
    std::string tmp;
    for (std::string::size_type i=0; i<a.length() && !x.abort; i++) tmp+=classdesc::xml_quote(a[i]);
    x.pack(d,tmp);
  }

  inline void xml_pack(xml_pack_t& x,const string& d, const std::string& a) 
  {xml_pack(x,d,const_cast<std::string&>(a));}

  /* now define the array version  */
  template <class T> void xml_pack(xml_pack_t& x,const string& d,is_array ia,
                                     T& a, int dims,size_t ncopies,...) 
  {
    va_list ap;
    va_start(ap,ncopies);
    for (int i=1; i<dims; i++) ncopies*=va_arg(ap,int); //assume that 2 and higher D arrays dimensions are int
    va_end(ap);
    xml_pack_t::Tag tag(x,d);

    // element name is given by the type name
    string eName=typeName<T>().c_str();
    // strip leading namespace and qualifiers
    const char *e=eName.c_str()+eName.length();
    while (e!=eName.c_str() && *(e-1)!=' ' && *(e-1)!=':') e--;

    for (size_t i=0; i<ncopies; i++) xml_pack(x,d+"."+e,(&a)[i]);
  }

  template <class T1, class T2>
  void xml_pack(xml_pack_t& x, const string& d, const std::pair<T1,T2>& arg)
  {
    xml_pack_t::Tag t(x,d);
    xml_pack(x,d+".first",arg.first);
    xml_pack(x,d+".second",arg.second);
  }  

  template <class T> typename
  enable_if<Or<is_sequence<T>, is_associative_container<T> >, void>::T
  xml_packp(xml_pack_t& x, const string& d, T& arg, dummy<1> dum=0)
  {
    xml_pack_t::Tag tag(x,d); 
    // element name is given by the type name
    string eName=typeName<typename T::value_type>().c_str();
    eName=eName.substr(0,eName.find('<')); //trim off any template args
    // strip leading namespace and qualifiers
    const char *e=eName.c_str()+eName.length();
    while (e!=eName.c_str() && *(e-1)!=' ' && *(e-1)!=':') e--;

    for (typename T::const_iterator i=arg.begin(); i!=arg.end(); ++i)
      xml_pack(x,d+"."+e,*i);
  }

  template <class T>
  void xml_pack_onbase(xml_pack_t& x,const string& d,T& a)
  {xml_pack(x,d+basename<T>(),a);}

  /* const static members */
  template<class T>
  void//typename enable_if<Not<is_pointer<T> >,void>::T
  xml_pack(xml_pack_t& targ, const string& desc, is_const_static i, T arg) 
  {} 

  template<class T>
  void xml_pack(xml_pack_t& targ, const string& desc, Exclude<T>&) {} 

  // special handling of shared pointers to avoid a double wrapping problem
  template<class T>
  void xml_pack(xml_pack_t& x, const string& d, shared_ptr<T>& a);

  template<class T>
  void xml_pack(xml_pack_t& targ, const string& desc, is_graphnode, T&)
  {
    throw exception("xml_pack of arbitrary graphs not supported");
  } 

}

#include "use_mbr_pointers.h"
CLASSDESC_USE_OLDSTYLE_MEMBER_OBJECTS(xml_pack)
CLASSDESC_FUNCTION_NOP(xml_pack)

using classdesc::xml_pack;
using classdesc::xml_pack_onbase;

#ifndef XML_COMMON_CD
#define XML_COMMON_CD
#include "xml_common.cd"
#endif

#endif

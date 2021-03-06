/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef POLYRESTPROCESS_H
#define POLYRESTPROCESS_H
#include "RESTProcess_base.h"

namespace classdesc
{
  struct PolyRESTProcessBase
  {
    virtual void RESTProcess(RESTProcess_t&, const string&) const=0;
    virtual void RESTProcess(RESTProcess_t&, const string&)=0;
    virtual ~PolyRESTProcessBase() {}
  };

  template <class T, class Base=PolyRESTProcessBase>
  struct PolyRESTProcess: public Base
  {
    void RESTProcess(RESTProcess_t& rp, const string& d) const override
    {::RESTProcess(rp,d,dynamic_cast<const T&>(*this));}
    void RESTProcess(RESTProcess_t& rp, const string& d) override
    {::RESTProcess(rp,d,dynamic_cast<T&>(*this));}
  };

  template <> struct tn<classdesc::PolyRESTProcessBase>
  {
    static std::string name()
    {return "classdesc::PolyRESTProcessBase";}
  };
  template <class T> struct tn<classdesc::PolyRESTProcess<T> >
  {
    static std::string name()
    {return "classdesc::PolyRESTProcess<"+typeName<T>()+">";}
  };

  template <class T>
  typename enable_if<is_base_of<PolyRESTProcessBase, T>, json_pack_t>::T
  rProcess(T& a, const string& remainder, const json_pack_t& arguments)
  {
    RESTProcess_t r;
    a.RESTProcess(r,"");
    auto i=r.find("");
    if (i!=r.end())
      return i->second->process(remainder,arguments);
    else
      return {};
  }

  template <class T>
  typename enable_if<Not<is_base_of<PolyRESTProcessBase, T>>, json_pack_t>::T
  rProcess(T& a, const string& remainder, const json_pack_t& arguments)
  {
    return RESTProcessObject<T>(a).process(remainder, arguments);
  }



}

#include "polyRESTProcess.cd"
#endif
      

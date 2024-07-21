/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/
#ifndef RESTPROCESS_EPILOGUE_H
#define RESTPROCESS_EPILOGUE_H
#include "RESTProcess_base.h"
#include "signature.h"
#include "multiArray.h"
#include "polyPackBase.h"
#include "polyRESTProcess.h"

namespace classdesc_access
{
  namespace classdesc=cd;

  template <class T>
  struct access_RESTProcess<cd::Exclude<T>>: public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <>
  struct access_RESTProcess<cd::string>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {repo.add(d, new cd::RESTProcessObject<U>(a));}
  };

  template <class F, class S>
  struct access_RESTProcess<std::pair<F,S>>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {
      RESTProcess(repo,d+".first", a.first);
      RESTProcess(repo,d+".second", a.second);
    }
  };

  template <>
  struct access_RESTProcess<cd::PolyPackBase>: 
    public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <class T>
  struct access_RESTProcess<cd::PolyPack<T>>: 
    public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <class T>
  struct access_RESTProcess<T, cd::void_t<typename std::iterator_traits<T>::value_type>>:
    public cd::NullDescriptor<cd::RESTProcess_t> {};

}

namespace classdesc
{
  template <class T> 
  REST_PROCESS_BUFFER RESTProcessObject<T>::signature() const
  {
    REST_PROCESS_BUFFER r;
    std::vector<Signature> signature
      {{typeName<T>(),{}},{typeName<T>(),{typeName<T>()}}};
    return r<<signature;
  }

  template <class T> 
  REST_PROCESS_BUFFER RESTProcessPtr<T>::signature() const
  {
    REST_PROCESS_BUFFER r;
    auto tn=typeName<typename T::element_type>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return r<<signature;
  }
  
  template <class T> 
  REST_PROCESS_BUFFER RESTProcessWeakPtr<T>::signature() const
  {
    json_pack_t r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return REST_PROCESS_BUFFER(r<<signature);
  }

  template <class T> 
  REST_PROCESS_BUFFER RESTProcessSequence<T>::signature() const
  {
    REST_PROCESS_BUFFER r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return REST_PROCESS_BUFFER(r<<signature);
  }

  template <class T> 
  REST_PROCESS_BUFFER RESTProcessAssociativeContainer<T>::signature() const
  {
    REST_PROCESS_BUFFER r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return REST_PROCESS_BUFFER(r<<signature);
  }

  template <class E> 
  REST_PROCESS_BUFFER RESTProcessEnum<E>::signature() const
  {
    REST_PROCESS_BUFFER r;
    std::vector<Signature> signature
      {{"std::string",{}},{"std::string",{"std::string"}}};
    return REST_PROCESS_BUFFER(r<<signature);
  }

  template <class T> 
  REST_PROCESS_BUFFER RESTProcessObject<T>::list() const
  {
    RESTProcess_t map;
    RESTProcess(map,"",obj);
    REST_PROCESS_BUFFER::Array array;
    for (auto& i:map)
      if (!i.first.empty())
        array.emplace_back(i.first);
    return REST_PROCESS_BUFFER(array);
  }

  
  template <class F, int N=functional::Arity<F>::value >
  struct Args: public std::vector<string>
  {
    Args(): std::vector<string>(Args<F,N-1>()) {
      push_back(typeName<typename functional::Arg<F,N>::type>());
    }
  };

  template <class F> struct Args<F,0>: public std::vector<string> {};

  
  template <class F>
  REST_PROCESS_BUFFER RESTProcessBase::functionSignature() const
  {
    REST_PROCESS_BUFFER r;
    return REST_PROCESS_BUFFER(r<<Signature{typeName<typename functional::Return<F>::T>(), Args<F>()});
  }

  template <class T>
  typename enable_if<is_classdescGenerated<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, string d, T& obj)
  {
    classdesc_access::access_RESTProcess<typename remove_const<T>::type>()(repo,d,obj);
    repo.add(d, new RESTProcessObject<T>(obj));
  }

  template <class T>
  REST_PROCESS_BUFFER RESTProcessPtr<T>::process(const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    if (ptr)
      return rProcess(*ptr, remainder, arguments);
    else
      return {};
  }

  template <class E>
  REST_PROCESS_BUFFER EnumerateEnumerators<E>::process
  (const string& remainder, const REST_PROCESS_BUFFER& arguments)
    {
      auto& enumerators=enum_keys<E>();
      std::vector<string> tmp(enumerators.sbegin(), enumerators.send());
      return REST_PROCESS_BUFFER(tmp);
    }

  
  template <class T>
  REST_PROCESS_BUFFER RESTProcessWeakPtr<T>::process
  (const string& remainder, const REST_PROCESS_BUFFER& arguments)
  {
    if (auto p=ptr.lock())
      return rProcess(*p, remainder, arguments);
    else
      return {};
  }

  inline REST_PROCESS_BUFFER RESTProcess_t::process(const std::string& query, const REST_PROCESS_BUFFER& jin)
    {
      string cmd=query;

      if (cmd=="@enum.@list")
        {
          std::vector<string> enums;
          for (auto& i: *this)
            if (i.first.find("@enum")==0)
              enums.push_back(i.first.substr(6));
          REST_PROCESS_BUFFER r;
          r<<enums;
          return r;
        }

      for (auto cmdEnd=query.length(); ;
           cmdEnd=cmd.rfind('.'), cmd=cmd.substr(0,cmdEnd))
        {
          if (cmdEnd==string::npos)
            {
              cmdEnd=0;
              cmd="";
            }
          auto tail=query.substr(cmdEnd);
          switch (count(cmd))
            {
            case 0:
              if (cmdEnd)
                continue; // try next split
              else
                throw std::runtime_error("Command not found: "+query);
            case 1: // simple object or non overloaded function
              {
                auto r=find(cmd);
                if (tail==".@signature")
                  return r->second->signature();
                else if (tail==".@list")
                  return r->second->list();
                else if (tail==".@type")
                  return r->second->type();
                else if (cmdEnd || dynamic_cast<RESTProcessWrapperBase*>(r->second.get()))
                  return r->second->process(tail, jin);
                else
                  throw std::runtime_error("Command not found: "+query);
              }
            default:
              {
                auto r=equal_range(cmd);
                if (tail==".@signature")
                  {
                    std::vector<Signature> array;
                    for (; r.first!=r.second; ++r.first)
                      {
                        array.emplace_back();
                        r.first->second->signature()>>array.back();
                      }
                    REST_PROCESS_BUFFER r;
                    r<<array;
                    return r;
                  }
                // sort function overloads by best match
                auto cmp=[&](RESTProcessFunctionBase*x, RESTProcessFunctionBase*y)
                {return x->matchScore(jin)<y->matchScore(jin);};
                std::set<RESTProcessFunctionBase*, decltype(cmp)> sortedOverloads{cmp};
                for (auto i=r.first; i!=r.second; ++i)
                  if (auto j=dynamic_cast<RESTProcessFunctionBase*>(i->second.get()))
                    sortedOverloads.insert(j);
                auto& bestOverload=*sortedOverloads.begin();
                if (bestOverload->matchScore(jin) >=
                    RESTProcessFunctionBase::maxMatchScore)
                  throw std::runtime_error("No suitable matching overload found");
                if (sortedOverloads.size()>1)
                  { // ambiguous overload detection
                    auto i=sortedOverloads.begin(); i++;
                    if ((*i)->matchScore(jin)==bestOverload->matchScore(jin))
                      throw std::runtime_error("Ambiguous resolution of overloaded function");
                  }
                if (tail==".@list")
                  return bestOverload->list();
                if (tail==".@type")
                  return bestOverload->type();
                return bestOverload->process(tail, jin);
              }
            }
        }
    }

  
  
  template <class T> typename enable_if<And<is_class<T>, Not<is_container<T>>>,void>::T
  populateFromObj(RESTProcess_t& r, T&obj)
  {classdesc_access::access_RESTProcess<T>()(r,"",obj);}
  
  template <class T> typename enable_if<Or<Not<is_class<T>>, is_container<T>>,void>::T
  populateFromObj(RESTProcess_t& r, T&obj)
  {}
  
  template <class T> RESTProcess_t::RESTProcess_t(T&obj) {populateFromObj(*this,obj);}

  template <class T> struct RESTProcessHeapObject:  public RESTProcessPtr<std::unique_ptr<T>>
  {
    std::unique_ptr<T> obj;
    RESTProcessHeapObject(): RESTProcessPtr<std::unique_ptr<T>>(obj) {}
  };
  
  template <class T, class...A> void RESTProcess_t::addFactory
  (const string& typeName,const std::function<void(const std::string& objName)>& callback)
  {
    std::function<void(const std::string& name, A... args)> factory=
      [this,callback](const std::string& name, A... args) {
      auto rp=std::make_unique<RESTProcessHeapObject<T>>();
      rp->obj=std::make_unique<T>(std::forward<A>(args)...);
      add(name,rp.release());
      callback(name);
    };
    
    add(typeName, new RESTProcessFunction<decltype(factory),void>(factory));
  }

}

namespace classdesc_access
{
  namespace cd=classdesc;
#if defined(__cplusplus) && __cplusplus>=201103L
  template <class F>
  struct access_RESTProcess<std::function<F>>
  {
    void operator()(cd::RESTProcess_t& r, const cd::string& d, const std::function<F>& a)
    {r.add(d, new cd::RESTProcessFunction<std::function<F>>(a));}
  };
#endif
}
    
#endif

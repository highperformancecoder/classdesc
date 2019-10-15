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

namespace classdesc_access
{
  namespace classdesc=cd;

  template <class T>
  struct access_RESTProcess<cd::Exclude<T>>: public cd::NullDescriptor<cd::RESTProcess_t> {};

  template <class T>
  struct access_RESTProcess<cd::shared_ptr<T>>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {repo.add(d, new cd::RESTProcessPtr<U>(a));}
  };

  template <class T>
  struct access_RESTProcess<cd::weak_ptr<T>>
  {
    template <class U>
    void operator()(cd::RESTProcess_t& repo, const std::string& d, U& a)
    {repo.add(d, new cd::RESTProcessPtr<U>(a));}
  };

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

}

namespace classdesc
{
  template <class T> 
  json_pack_t RESTProcessObject<T>::signature() const
  {
    json_pack_t r;
    std::vector<Signature> signature
      {{typeName<T>(),{}},{typeName<T>(),{typeName<T>()}}};
    return r<<signature;
  }

  template <class T> 
  json_pack_t RESTProcessPtr<T>::signature() const
  {
    json_pack_t r;
    auto tn=typeName<typename T::element_type>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return r<<signature;
  }
  
  template <class T> 
  json_pack_t RESTProcessPtr<std::weak_ptr<T>>::signature() const
  {
    json_pack_t r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return r<<signature;
  }

  template <class T> 
  json_pack_t RESTProcessSequence<T>::signature() const
  {
    json_pack_t r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return r<<signature;
  }

  template <class T> 
  json_pack_t RESTProcessAssociativeContainer<T>::signature() const
  {
    json_pack_t r;
    auto tn=typeName<T>();
    std::vector<Signature> signature{{tn,{}},{tn,{tn}}};
    return r<<signature;
  }

  template <class E> 
  json_pack_t RESTProcessEnum<E>::signature() const
  {
    json_pack_t r;
    std::vector<Signature> signature
      {{"std::string",{}},{"std::string",{"std::string"}}};
    return r<<signature;
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
  json_pack_t RESTProcessBase::functionSignature() const
  {
    json_pack_t r;
    return r<<Signature{typeName<typename functional::Return<F>::T>(), Args<F>()};
  }

  template <class T>
  typename enable_if<is_classdescGenerated<T>, void>::T
  RESTProcessp(RESTProcess_t& repo, string d, T& obj)
  {
    classdesc_access::access_RESTProcess<typename remove_const<T>::type>()(repo,d,obj);
    repo.add(d, new RESTProcessObject<T>(obj));
  }
  

}

#endif

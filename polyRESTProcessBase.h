/*
  @copyright Russell Standish 2021
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef POLYRESTPROCESSBASE_H
#define POLYRESTPROCESSBASE_H
namespace classdesc
{
  struct RESTProcess_t;
  struct PolyRESTProcessBase
  {
    virtual void RESTProcess(RESTProcess_t&, const std::string&) const=0;
    virtual void RESTProcess(RESTProcess_t&, const std::string&)=0;
    virtual ~PolyRESTProcessBase() {}
  };
}
#endif

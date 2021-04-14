/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#ifndef RESTPROCESSEXAMPLE_H
#define RESTPROCESSEXAMPLE_H
#include <classdesc.h>
#include <stringKeyMap.h>
#include <polyRESTProcess.h>
using namespace classdesc;

#include <string>
#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <string.h>

enum EnumFoo {ea, eb, ec=12, ed};

string printHello() {return "hello";}

struct Foo
{
  const static int csi=20;
  static int si;
  char ch;
  double a;
  float af;
  int b;
  bool bf, bt;
  std::string c;
  std::vector<std::string> c1;
  int d[3];
  int d1[3][2];
  std::vector<int> h;
  std::list<int> l;
  std::map<int,int> m;
  std::unordered_map<int,int> um;
  std::list<std::list<std::string> > llex;
  Exclude<int> iex;
  std::vector<string> vs;
  EnumFoo ef;
  std::shared_ptr<EnumFoo> sef;
  //string (*hello)(); // still not working ...
  classdesc::StringKeyMap<int> sm;
  Foo() {}
  Foo(int i): ch('M'), a(0.1), af(0.2), b(i), bf(false), bt(true),
              c("\r hello & 123 "), c1(2,"\r"), h(3,2), l(3,2), 
              llex(2,std::list<std::string>(2,"hello")),
              vs(2," hello"), ef(ea), sef(new EnumFoo(ea))//, hello(printHello)
  {
    for (int i=0; i<3; i++) 
      {
        d[i]=i; 
        for (int j=0; j<2; ++j)
          d1[i][j]=2*i+j;
      }
    m[0]=5; m[3]=2;
  }
  string vs0() const {return vs[0];}
  static string shello() {return "hello";}
  int getEF() {return ef;}
  string getc() {return c;}

  size_t seqLength(const std::vector<int>& x) {
    return x.size();
  }
  std::vector<string> getVS() const {return vs;}
  virtual std::string name() const {return "Foo";}
  // function pointer member not yet supported
//  int (Foo::* memPtrEx)(int);
//  int (*ptrEx)(int);
};

enum GlobE {ga,gb};

struct Bar: Foo
{
  enum BarE {a, b};
  Bar(const Bar&)=delete;
  int f;
  EnumFoo barfoo;
  std::vector<Foo> vFoo;
  std::map<std::string,Foo> mFoo;
  Bar& overloadExample() {return *this;}
  int overloadExample(int x, int y=0) const {return x+y;}
  // test overloading
  BarE barE(BarE e) const {return e;}
  BarE barE() const {return a;}
  GlobE globE() const {return ga;}
  Bar() {}
  Bar(int i): Foo(i), f(20), barfoo(eb), vFoo(3,1) {mFoo["foo"]=Foo(2);}

  // overload triggers bug
  void rvalueRef(Foo&& f) {}
  void rvalueRef() {}

  std::string name() const override {return "Bar";}
  Foo& foo() {return *this;}

  // issue with template return args - see ticket #68
  // std::vector<std::vector<Foo>> templateReturnIssue() {return {};};
  std::vector<std::vector<Foo> > templateReturnIssue() {return {};};
};

struct Bar1
{
  Foo f;
  shared_ptr<Foo> fp{new Foo};
  int g;
  EnumFoo barfoo;
  std::vector<Foo> vFoo;
  Bar1() {}
  Bar1(int i): f(i), g(2), barfoo(ec), vFoo(2,Foo(1)) {}
  Foo foo() {return f;}
  Foo& fooRef() {return f;}
  Foo* foop() {return &f;} // should be ignored, as can't determine ownership
  static Foo* sfoop() {return nullptr;}// should be ignored, as can't determine ownership
  static Foo* sfoop(int) {return nullptr;}// should be ignored, as can't determine ownership
  Bar1& recursiveType(const Bar1&) {return *this;} // tests a recursive type definition bug
  Bar1& recursiveType(const char* x) {return *this;} // tests another parsing bug
  Bar::BarE barE(Bar::BarE x) {return x;} // tests another parsing bug
};

struct FooBar1
{
  Foo f{0};
};

// extra class to test that argument types are automatically registered
struct DD
{
  int a;
};

// abstract and default constructorless tests
struct Abstract
{
  virtual int foo()=0;
  int bar() const {return 2;}
};

struct Defaultless: public Abstract
{
  Defaultless(int) {}
  int foo() override {return 0;}
};

// test of polymorphic types
struct IPoly: public PolyRESTProcess<IPoly> {};

struct Poly: public PolyRESTProcess<Poly, IPoly>
{
  int a;
};

// root type
struct Root
{
  Defaultless defaultless{1};
  Bar bar;
  Bar1 bar1;

  std::shared_ptr<IPoly> spoly{new Poly};
  std::shared_ptr<Bar> sbar{new Bar};
  
  FooBar1& getFB1() {
    static FooBar1 m;
    return m;
  }
  double fb1a() {return getFB1().f.a;}
  Root(): bar(3), bar1(2) {}
  void dummy(DD& x) {}
};

#include "RESTProcessExample.cd"
#endif

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <classdesc.h>
#include <stringKeyMap.h>
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
  std::list<std::list<std::string> > llex;
  Exclude<int> iex;
  std::vector<string> vs;
  EnumFoo ef;
  std::shared_ptr<EnumFoo> sef;
  //string (*hello)(); // still not working ...
  classdesc::StringKeyMap<int> sm;
  Foo() {}
  Foo(int i): ch('M'), a(0.1), af(0.2), b(3), bf(false), bt(true),
              c("\r hello & 123 "), c1(2,"\r"), h(3,2), l(3,2), 
              llex(2,std::list<std::string>(2,"hello")),
              vs(2," hello"), ef(ea)//, hello(printHello)
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
};

struct Bar: Foo
{
  Bar(const Bar&)=delete;
  int f;
  EnumFoo barfoo;
  std::vector<Foo> vFoo;
  Bar() {}
  Bar(int i): Foo(i), f(20), barfoo(eb), vFoo(3,1) {}
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
};

struct FooBar1
{
  Foo f{0};
};

// root type
struct Root
{
  Bar bar;
  Bar1 bar1;
  FooBar1& getFB1() {
    static FooBar1 m;
    return m;
  }
  double fb1a() {return getFB1().f.a;}
  Root(): bar(3), bar1(2) {}
};

#include "pythonExample.cd"

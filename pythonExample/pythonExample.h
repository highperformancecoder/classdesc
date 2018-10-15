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
  //  std::map<int,int> m;
  std::list<std::list<std::string> > llex;
  Exclude<int> iex;
  std::vector<string> vs;
  EnumFoo ef;
  //classdesc::shared_ptr<EnumFoo> sef;
  //string (*hello)();
  //  classdesc::StringKeyMap<int> sm;
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
    //    m[0]=5; m[3]=2;

//    sm["hello"]=2;
//    sm["goodbye"]=3;
  }
//  bool operator!=(const Foo& x) const {return ch!=x.ch || fabs(a-x.a)>1e-10 || b!=x.b ||bf !=x.bf || bt!=x.bt || c!=x.c ||c1!=x.c1|| memcmp(d,x.d,sizeof(d)) ||
//      memcmp(d1,x.d1,sizeof(d1))
//      || h!=x.h|| l!=x.l|| /*m!=x.m ||*/ vs!=x.vs /* || llex!=x.llex || */
//      /*(!sef && sef!=x.sef) || (sef && x.sef && *sef!=*x.sef)*/;}
//  bool operator==(const Foo& x) const {return !operator!=(x);}
  string vs0() const {return vs[0];}
  static string shello() {return "hello";}
  int getEF() {return ef;}
  string getc() {return c;}
};

struct Bar: Foo
{
  int f;
  EnumFoo barfoo;
  //  std::vector<Foo> vFoo;
  Bar() {}
  Bar(int i): Foo(i), f(20), barfoo(eb)/*, vFoo(3,1)*/ {}
  //  bool operator!=(const Bar& x) const {return Foo::operator!=(x)||f!=x.f||barfoo!=x.barfoo /*|| vFoo!=x.vFoo*/;}
};

struct Bar1
{
  Foo f;
  int g;
  EnumFoo barfoo;
  //  std::vector<Foo> vFoo;
  Bar1() {}
  Bar1(int i): f(i), g(2), barfoo(ec)/*, vFoo(2,Foo(1))*/ {}
//  bool operator!=(const Bar1& x) const {return f!=x.f||g!=x.g||barfoo!=x.barfoo
//      /*||vFoo!=x.vFoo*/;}
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

struct rootInit: classdesc::detail::PythonRef<Root>
{
  static Root root;
  rootInit(): classdesc::detail::PythonRef<Root>(root) {}
};


#include "pythonExample.cd"

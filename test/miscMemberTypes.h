/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

struct Foo
{
  // function pointer test
  void (*f)();
  typedef struct {int a; union {int b; float c;} aa;} Bar;
  typedef struct {int a;} *Bar2;
  typedef union {int b; float c;} *Bar3;
  typedef union {int b; float c;} Bar4;

  union {int a; float b;} m_union;

};

typedef struct {int b; float c;} Foo1;
typedef union {int b; float c;} Foo2;
typedef struct {int b; float c;} *Foo4;

template <class T>
struct Bar
{  
  // template class defs
  template <class U> struct X2 {};
  template <typename U> struct X3 {};
};
#include "miscMemberTypes.cd"

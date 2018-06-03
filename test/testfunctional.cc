/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <string>

#include "javaClassDescriptor.h"
#include "function.h"
#include "classdesc_epilogue.h"
#include <assert.h>
#include <iostream>
using namespace std;

using namespace classdesc;
using namespace classdesc::functional;

void foovv() {}
void foovi(int x) {assert(x==2);}
int fooiv() {return 1;}
int fooii(int x) {assert(x==2); return x;}
int fooif(float x) {assert(x==2.0); return int(x);}
int fooiif(int x, float y) {assert(x==2); assert(y==-1.0); return x;}

struct Foo
{
  int m1() {return 1;}
  int m2(int x) {assert(x==2); return x;}
  int m3(int x, float y) {assert(x==2); assert(y==-1.0); return x;}
};

int main()
{
  int args[]={2,-1};
  apply(NULL,&foovv,args);
  apply(NULL,&foovi,args);
  assert(apply_nonvoid_fn(&fooiv,args)==1);
  assert(apply_nonvoid_fn(&fooii,args)==2);
  assert(apply_nonvoid_fn(&fooif,args)==2);
  assert(apply_nonvoid_fn(&fooiif,args)==2);

  Foo a;
  assert(apply_nonvoid_fn(bindMethod(a,&Foo::m1),args)==1);
  assert(apply_nonvoid_fn(bindMethod(a,&Foo::m2),args)==2);
  assert(apply_nonvoid_fn(bindMethod(a,&Foo::m3),args)==2);

  assert(arg_description(&fooiv)=="");
  assert(arg_description(&fooii)=="I");
  assert(arg_description(&fooif)=="F");
  assert(arg_description(&fooiif)=="IF");
}

/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include "classdesc.h"
#include "testTypeName.h"
#include "testTypeName.cd"
#include "classdesc_epilogue.h"

#include <assert.h>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <iostream>
using namespace std;
using classdesc::typeName;
using classdesc::enumKey;

int main()
{
  cout << typeName<foo>() << endl;
  cout << typeName<bar>() << endl;
  cout << typeName<nn::dd>() << endl;
  cout << typeName<nn::foo>() << endl;
  cout << typeName<nn::bar<int> >() << endl;
  cout << typeName<foobar<int> >() << endl;

  cout << typeName<std::vector<double> >() << endl;
  cout << typeName<std::deque<double> >() << endl;
  cout << typeName<std::list<double> >() << endl;
  cout << typeName<std::set<double> >() << endl;
  cout << typeName<std::map<int,double> >() << endl;

  //  cout << enumKey<dd>("aa") << " " << enumKey<dd>(5) <<" "<< enumKey<dd>("cc") << endl;
  cout << enumKey<nn::dd>("aa") << " " << enumKey<nn::dd>(5) <<" "<< enumKey<nn::dd>("cc") << endl;

  nn::DD x, y;
  x.a=nn::DD::aa; y.a=nn::DD::bb;
  classdesc::pack_t b;
  b<<x;
  b>>y;
  assert(x.a==y.a);
};

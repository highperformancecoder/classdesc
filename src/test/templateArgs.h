/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <vector>
using namespace std;

template <class T, class U=vector<int> >
struct Foo
{};

template <> struct Foo<vector<int> > {};

#include "templateArgs.cd"

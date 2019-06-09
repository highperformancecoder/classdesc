/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

struct foo
{
struct bar;
};

struct foo::bar
{
int a;
};

template <class F>
struct barT
{
  struct foo;
};

template <class F>
struct barT<F>::foo
{
  int a;
};

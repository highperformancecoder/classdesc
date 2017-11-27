/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// test for ticket 12
namespace foo
{
  struct bar
  {
    enum Foo {x, y, z};
  };


  template <foo::bar::Foo t>
  class Foobar {};
}

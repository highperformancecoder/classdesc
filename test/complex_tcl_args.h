/*
  @copyright Russell Standish 2019
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// Illustrates a tricky situation that classdesc gets caught up on

namespace bar
{
  struct foo {};
}

struct foo
{
  enum EnumTest {ff, bar};
  EnumTest testEnum(int args) {return EnumTest(args);}
 
  bar::foo b;
};

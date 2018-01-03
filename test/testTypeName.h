/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

class foo
{};

struct bar: foo
{};

//enum dd {aa=1, bb=5, cc};

namespace nn
{
  class foo {};
  template <class T> class bar {};
  enum dd {aa=1, bb=5, cc};
  struct DD
  {
    enum dd {aa=1, bb=5, cc};
    dd a;
  };
}

template <class T> class foobar {};


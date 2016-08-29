/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/* This is an example of something classdesc can't quite cope with yet */

/*
template <class T>
class foo
{
public:
  class bar
  {
  public:
    double b;
  };
  bar a;
};
*/

template <class T>
class bart
{
public:
  double b;
};

template <class T>
class foo
{
public:
  typedef bart<T> bar;
  bar a;
};


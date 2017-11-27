/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <pack_base.h>
#if defined(__GCC__)
#define TRAD_STL 1
#endif
#include <pack_stl.h>
#include <classdesc_epilogue.h>
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
using namespace std;
using namespace classdesc;

template <class ForwardIterator, class V>
void range(ForwardIterator begin, ForwardIterator end, V val)
{for (ForwardIterator i=begin; i!=end; *i=val++, i++);}

// factories for dummy test object
template <class T> T gen();
template <> vector<int> gen<vector<int> >() 
{
  vector<int> x(10);
  range(x.begin(),x.end(),0);
  return x;
}

template <> list<int> gen<list<int> >() 
{
  list<int> x(10);
  range(x.begin(),x.end(),0);
  return x;
}

template <> deque<int> gen<deque<int> >() 
{
  deque<int> x(10);
  range(x.begin(),x.end(),0);
  return x;
}

template <> set<int> gen<set<int> >() 
{
  set<int> r;   
  for (int i=0; i<10; i++) r.insert(i);
  return r;
}

template <> multiset<int> gen<multiset<int> >() 
{
  multiset<int> r; 
  for (int i=0; i<10; i++) r.insert(i);
  return r;
}

template <> map<int,int> gen<map<int,int> >() {
  map<int,int> r; 
  for (int i=0; i<10; i++) r.insert(pair<int,int>(i,10-i));
  return r;
}

template <> multimap<int,int> gen<multimap<int,int> >() {
  multimap<int,int> r; 
  for (int i=0; i<10; i++) r.insert(pair<int,int>(i,10-i));
  return r;
}

template <class T>
void test_container()
{
  T x(gen<T>()), y;
  pack_t b;
  b<<x;
  b>>y;
  assert(x==y);
}

template <class T>
void test_iterator()
{
  T x=gen<T>();
  const T cx=gen<T>();
  Iterator<T> i0, i1, ix0(x), ix1(x);
  Iterator<const T> ci0, ci1, cix0(cx), cix1(cx);
  // should be nops
  {
    pack_t buf;
    buf<<i0<<ci0;
    buf>>i1>>ci1;
  }
  assert(i0==i1 && ci0==ci1);
  i0=x.begin(); ci0=cx.begin();
  {
    pack_t buf;
    buf<<i0<<ci0;
    buf>>i1>>ci1;
  }
  assert(Iterator<T>()==i1 && Iterator<const T>()==ci1);
 
  // these should advance the iterator a certain amount
  ix0=x.begin(); cix0=cx.begin();
  advance(ix0,3); advance(cix0,3);
  {
    pack_t buf;
    buf<<ix0<<cix0;
    buf>>ix1>>cix1;
  }
  assert(ix0==ix1 && cix0==cix1);
}

int main()
{
  test_container<vector<int> >();
  test_container<deque<int> >();
  test_container<list<int> >();
  test_container<set<int> >();
  test_container<multiset<int> >();
  test_container<map<int,int> >();
  test_container<multimap<int,int> >();

  {
    std::string x,y;
    x="hello";
    pack_t buf;
    buf << x; buf >> y;
    if (x!=y) {
      printf("error testing string\n");
      abort();
    }
  }
     

  test_iterator<vector<int> >();
  test_iterator<deque<int> >();
  test_iterator<list<int> >();
  test_iterator<map<int,int> >();
  test_iterator<set<int> >();

  return 0;  
}


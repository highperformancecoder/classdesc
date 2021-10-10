#include <function.h>
#include <pack_base.h>
#include <classdesc_epilogue.h>
#include <vector>

typedef std::vector<int> VI;
VI x(2);
int getX() {return x[0];}
int getXi(int i) {return x[i];}
void setX(int y) {x[0]=y;}
void setXi(int i, int y) {x[i]=y;}
void setXii(int i, int y, int z) {x[i]=y*z;}
void clearX() {x.clear();}

// vector methods factory



using namespace classdesc;
using namespace classdesc::functional;

int main()
{
  PackFunctor<pack_t> pf;
  pf<<2;
  assert(x[0]==0);
  pf.call(setX);
  assert(x[0]==2);
  assert(pf.call(getX)==x[0]);

  pf.clear();
  pf<<1<<3;
   assert(x[1]==0);
  pf.call(setXi);
  assert(x[1]==3);
  pf.clear();
  pf<<1;
  assert(pf.call(getXi)==3);

  pf.clear();
  pf<<1<<3<<4;
  x[1]=0;
  pf.call(setXii);
  assert(x[1]==12);

  pf.call(clearX);
  assert(x.empty());
  
  pf.clear();

//  // now try calling vectors methods via the Command pattern
//  void (VI::*resize)(size_t)=&VI::resize;
//  int& (VI::*at)(size_t)=&VI::at;
//  MethodPackFunctor<VI,pack_t> mpf(resize,at,&VI::size,&VI::clear);
//  mpf(resize,2);
//  mpf.invoke(x);
//  assert(x.size()==2);
//  mpf(&VI::size);
//  auto r=mpf.invoke(x);
//  assert(r->getResult<size_t>()==2);
//  x[1]=10;
//  mpf(at,1);
//  r=mpf.invoke(x);
//  assert(r->getResult<int&>()==x[1]);
//  mpf(&VI::clear);
//  mpf.invoke(x);
//  assert(x.empty());
}  
  

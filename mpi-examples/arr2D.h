/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <vector>
#include <exception>
using std::vector;

#include <stdexcept>
using std::runtime_error;

#include "pack_base.h"
#include "classdesc_access.h"
using namespace classdesc;

template <class T>
class arr2D
{
  vector<T> vals;
  CLASSDESC_ACCESS(arr2D);
public:
  int nx, ny, offsx, offsy;
  arr2D() {nx=ny=0;}
  arr2D(int x, int y, int ox=0, int oy=0) {dimension(x,y,ox,oy);}
  //  arr2D(const arr2D& x);
  /* second two arguments are origin offsets for nonzero-based indexing */
  void dimension(int x, int y, int ox=0, int oy=0);
  T& operator()(int i, int j);
  arr2D subarray(int i0, int j0, int nx, int ny);
  /* insert a subarray into *this, offset at i,j */
  void insert(const arr2D& x, int i0, int j0);
};


template <class T>
inline void arr2D<T>::dimension(int x, int y, int ox, int oy) 
{
  vals=vector<T>(x*y);
  nx=x; ny=y; 
  offsx=ox; offsy=oy;
}

template <class T>
inline arr2D<T> arr2D<T>::subarray(int i0, int j0, int nx, int ny)
{
  arr2D r(nx,ny);
  for (int i=0; i<nx; i++)
    for (int j=0; j<ny; j++)
      r(i,j)=(*this)(i+i0,j+j0);
  return r;
}

template <class T>
inline void arr2D<T>::insert(const arr2D<T>& x, int i0, int j0)
{
  if (i0+x.nx+x.offsx > nx+offsx || j0+x.ny+x.offsy > ny+offsy)
    throw runtime_error("Inserting too large a subarray");
  for (int i=x.offsx; i<x.nx+x.offsx; i++)
    for (int j=x.offsy; j<x.ny+x.offsy; j++)
      (*this)(i+i0,j+j0)=const_cast<arr2D<T>&>(x)(i,j);
}

template <class T>
inline T& arr2D<T>::operator()(int i, int j)
{
  if (i<offsx) i+=nx;
  else if (i>=offsx+nx) i-=nx;
  if (j<offsy) j+=ny;
  else if (j>=offsy+ny) j-=ny;
  return vals[i-offsx+(j-offsy)*nx];
}



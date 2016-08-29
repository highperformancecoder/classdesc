/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <mpi.h>
#include <complex>
#include <vector>
#include "pack_stl.h"
#include "classdescMP.h"
#include "classdesc_epilogue.h"
#include <iostream>
using namespace std;
using namespace classdesc;

const int chunk=64;
const int nx=256;
const int ny=chunk*4; /* ensure ny is a multiple of chunk */
const int maxiter=1000;

struct S
{
  void get_counts(MPIbuf& args)
  {
    int ix,i,j,k, minj;
    args >> ix;

    vector<int> ret_count(chunk);
    complex<double> z, kappa;

    i=ix/ny; minj=ix%ny;
    for (j=minj; j<minj+chunk; j++)
      {
	z=kappa=complex<double>((4.0*(i-nx/2))/nx, (4.0*(j-ny/2)/ny));
	for (k=0; ; k++)
	  {
	    if (abs(z)>2 || k==maxiter)
	      {
		ret_count[j-minj]=(k*256)/maxiter;
		break;
	      }
	    z=z*z+kappa;
	  }
      }
    args.reset() << ix << ret_count;
  }
};

  extern "C" {
    void raster(int *,int,int);
    void raster_pause();
  }

int main(int argc, char *argv[])
{
  try {
  int T[nx*ny];
  int p, rij;
  vector<int> ret_count(chunk);
  MPISPMD parallelEnvironment(argc,argv);
  MPIslave<S> slave;

  { /* parallel code here */

    if (slave.nprocs()<2)
      {
	cerr << "need at least 2 processors for master-slave" << endl;
	abort();
      }

    // this chunk of code does nothing other than exercise the bcast method
    slave.bcast(slave << &S::get_counts << 0);


    /* hand out work */
    for (int ij=0; ij<nx*ny; ij+=chunk)
      {
	if (!slave.idle.size())
	  {
	    slave.get_returnv()>> rij >> ret_count;
	    for(size_t i=0; i<ret_count.size(); i++) T[rij+i]=ret_count[i];
	  }
	slave.exec(slave << &S::get_counts << ij);
      }

    /* process final computations */
    while (!slave.all_idle())
	  {
	    slave.get_returnv()>> rij >> ret_count;
	    for(size_t i=0; i<ret_count.size(); i++) T[rij+i]=ret_count[i];
	  }

  }    /* end of parallel processing region */


#if 1
#ifndef AEGIS_TEST
  /* raster code for visualising results */
  raster(T,nx,ny);
  raster_pause();
#endif
#endif

#ifdef AEGIS_TEST
  /* this section is used as an AEGIS test */
  int T1[nx*ny], ij;
  S s;
  for (ij=0; ij<nx*ny; ij+=chunk)
    {
      MPIbuf b; b<<ij;
      s.get_counts(b);
      b >> rij >> ret_count;
      for(int i=0; i<ret_count.size(); i++) T1[rij+i]=ret_count[i];
    }
  int same=1;
  for (ij=0; same && ij<nx*ny; ij++) same &= T[ij]==T1[ij];
  if (!same) 
    {
      cerr << "parallel and serial version differ at ij=" << ij << endl; 
      abort();
    }
#endif
  }
  catch (runtime_error x) {cerr <<x.what()<<endl; abort();}
  return 0;
}

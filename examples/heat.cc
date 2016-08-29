/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <stdio.h>
#include "arr2D.h"
#include "pack_stl.h"
#include "arr2D.cd"
#include "classdesc_epilogue.h"

#include <vector>

using namespace std;

const int nx=100, ny=100;
const double kappa=0.1;
const char* checkname="checkpoint";

extern "C" {
  void raster(int [nx][ny],int,int);
  void raster_pause();
}

using namespace classdesc;

int main(int argc, char *argv[])
{
  int i,j, ntsteps;
  arr2D<double> T(nx,ny), nT(nx,ny);
  void pack(pack_t*,classdesc::string,arr2D<double>);

  if (argc<2) 
    {
      puts("usage heat <no. timsteps> [restart]");
      return 0;
    }

  ntsteps=atoi(argv[1]);

  if (argc>2 && strcmp(argv[2],"restart")==0)
    {
      xdr_pack sb(checkname,"r"); 
      sb >> T;
    }
  else
    {  /* initialise to delta function */
      for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	  T(i,j)=0;
      T(nx/2,ny/2)=10000;
    }

  for (int tstep=0; tstep<ntsteps; tstep++)
    {
      /* update grid */
      for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	  nT(i,j) = (1-4*kappa)*T(i,j) + 
	  kappa*( T(i-1,j) + T(i+1,j) + T(i,j-1) + T(i,j+1));
      T=nT;

#if 0  /* display heat evolution */
      int iT[nx][ny];
      for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	  iT[i][j]=(int)(T(i,j)/10);
      raster(iT,nx,ny);
#endif     

    }

  /* write checkpoint file */
  {
    xdr_pack b(checkname,"w"); b << T;
  }

  /* dump T to stdout for testing purposes */
  for (i=0; i<nx; i++)
    for (j=0; j<ny; j++)
      printf("%f\n",T(i,j));

  return 0;
}



/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <mpi.h>
#include <iostream>
using namespace std;
#include <math.h>
#include "arr2D.h"
#include "pack_stl.h"
#include "arr2D.cd"
#include "classdescMP.h"
#include "classdesc_epilogue.h"


const int nx=100, ny=100;
const double kappa=0.1;

arr2D<double> gT;  /* global array */

struct heat {
  arr2D<double> T;
  int left, right, up, down;
  int nprocs, myid;
  /*
     dimensions of local T subarray,
     processor rank index in x and y dimension,
     i & j offsets in global temperature array for T(0,0)
  */
  int nx, ny, px, py, ioffs, joffs;

  heat()
  {
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    px = (int) sqrt( (double) nprocs);
    py=nprocs/px;
    if (px*py!=nprocs)
    { printf("%d: warning %d processor(s) unused.\n", myid, nprocs-px*py);
    }
  }

  /* ensure we have a periodic mod function */
  int mod(int i, int j) { return i>=0? i%j: (i+(-i/j+1)*j)%j;}

  /* return the subarray size, and offsets into gT for each processor */
  void distribute(int myid, int& nx, int& ny, int& i, int& j)
  {
    //    printf("dist[id,nx,ny]=[%d,%d,%d]",myid,nx,ny);
    int xid        = myid % px;
    int yid        = myid / px;
    int old_nx     = nx;
    int old_ny     = ny;
    int xremainder = nx % px;
    int yremainder = ny % py;

    if (myid <= px*py-1)
    {  if (xid < xremainder) nx=nx/px+1; else nx=nx/px;
       if (yid < yremainder) ny=ny/py+1; else ny=ny/py;
       // i = nx*xid + (xid>=nx % px ? nx % px: 0);
       // j = ny*yid + (yid>=ny % py ? ny % py: 0);
       i = old_nx/px*xid + (xid >= xremainder ? xremainder: xid);
       j = old_ny/py*yid + (yid >= yremainder ? yremainder: yid);
       //       printf("  [id,xid,yid,nx,ny,i,j] = [%d,%d,%d,%d,%d,%d,%d]\n",myid,xid,yid,nx,ny,i,j);
    }
    else
    {  nx = 0; ny = 0; i = 0; j = 0; }
  }

  void init(arr2D<double>& gT)
  {
    MPIbuf b;
    b << gT.nx << gT.ny << bcast(0);
    b >> nx >> ny;
    distribute(myid,nx,ny,ioffs,joffs);

    /* distribute gT across processors */
    T.dimension(nx+2,ny+2,-1,-1); /* allow room for boundary elements */
    MPIbuf Tbuf;
    if (myid==0)
      {
	for (int i=0; i<nprocs; i++)
	  {
	    int nx=gT.nx, ny=gT.ny, io, jo;
	    distribute(i,nx,ny,io,jo);
	    Tbuf << gT.subarray(io,jo,nx,ny) << mark();
	  }
      }

    arr2D<double> tempT;
    Tbuf.scatter(0) >> tempT; /* unpack in temporary array, as not conformant*/
    T.insert(tempT,0,0);

    int xid=myid % px, yid=myid / px;
    left = mod(xid-1,px)+px*yid;
    right= mod(xid+1,px)+px*yid;
    up   = xid+px*mod(yid-1,py);
    down = xid+px*mod(yid+1,py);
    if (myid > px*py-1)
    { left = myid; right = myid; up = myid; down = myid; }
    //printf("[id,L,R,U,D] = [%d,%d,%d,%d,%d]\n", myid, left, right, up, down);
  }

  void update()
  {
    //printf("Update beg: myid=%d\n",myid);
    /* exchange boundary values */
    MPIbuf b;
    int i,j;
    for (i=0; i<ny; i++)  b << T(0,i);
    b.send_recv(left,0,right);
    for (i=0; i<ny; i++)  b >> T(nx,i);
    b.reset();

    for (i=0; i<ny; i++)  b << T(nx-1,i);
    b.send_recv(right,0,left);
    for (i=0; i<ny; i++)  b >> T(-1,i);
    b.reset();

    for (i=0; i<nx; i++)  b << T(i,0);
    b.send_recv(up,0,down);
    for (i=0; i<nx; i++)  b >> T(i,ny);
    b.reset();

    for (i=0; i<nx; i++)  b << T(i,ny-1);
    b.send_recv(down,0,up);
    for (i=0; i<nx; i++)  b >> T(i,-1);
    b.reset();

    /* update grid */
    arr2D<double> nT(nx,ny);
    for (i=0; i<nx; i++)
      for (j=0; j<ny; j++)
	nT(i,j) = (1-4*kappa)*T(i,j) +
	  kappa*( T(i-1,j) + T(i+1,j) + T(i,j-1) + T(i,j+1));
    T.insert(nT,0,0);
  }

  void get(arr2D<double>& gT)
  {
    MPIbuf a;
    a << T.subarray(0,0,nx,ny) << ioffs << joffs;
    a.gather(0);
    if (myid==0)
      {
	/* now assemble full temperature array from collected subarrays */
	arr2D<double> t;
	int i, j;
	for (int p=0; p<nprocs; p++)
	  {
	    a >> t>> i >> j;
	    gT.insert(t,i,j);
	  }
      }
  }
};

extern "C" {
  void raster(int [nx][ny],int,int);
  void raster_pause();
}

int main(int argc, char *argv[])
{
  int i,j;

  MPISPMD proc(argc,argv);
  heat H;

  if (proc.myid==0)
    {
      gT.dimension(nx,ny);
      for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	  gT(i,j)=0;
      gT(nx/2,ny/2)     =1000000;      // 5 heat sources
      gT(nx/4,ny/4)     =1000000;
      gT(nx/4,ny/4*3)   =1000000;
      gT(nx/4*3,ny/4)   =1000000;
      gT(nx/4*3,ny/4*3) =1000000;
    }

  H.init(gT);

#ifndef AEGIS_TEST
  int x = 0;
  for (;;)
    {
      int iT[nx][ny];
      H.get(gT);

      if (proc.myid==0)
	{
	  for (i=0; i<nx; i++)
	    for (j=0; j<ny; j++)
	      iT[i][j]=(int)(gT(i,j)/10);
	  raster(iT,nx,ny);
	}
      H.update();
      MPI_Barrier(MPI_COMM_WORLD);          // synchronisation point
      //x++; printf("%d: loop=%d nprocs=%d\n", H.myid, x, H.nprocs);
    }

#else  /* AEGIS_TEST */
  /* run model for 10 timesteps, and print out result */
  for (i=0; i<10; i++) H.update();
  H.get(gT);
  if (proc.myid==0)
    for (i=0; i<nx; i++)
      for (j=0; j<ny; j++)
	cout << i << ' ' << j << ' ' <<gT(i,j) << endl;
#endif
  return 0;
}



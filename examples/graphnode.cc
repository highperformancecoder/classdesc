/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

/* this example exercises the graphnode functionality. An NxN grid of
nodes is created, connected toroidally, so there are plenty of
loops. Then it is packed into a pack_t, and exploded out into a second
graph. Finally, the graph's stucture is checked to be identical to the
first, but occupying a different part of the heap.  */

#include <stdio.h>
#include "pack_base.h"
#include "pack_graph.h"

#include "graphnode.h"
#include "graphnode.cd"
#include "classdesc_epilogue.h"

#include <memory>
using namespace std;

using namespace classdesc;

int foonode::nodecntr=0;

// register in static object to get around leak detector
vector<shared_ptr<foonode> > store;

foonode* newFoonode() {
  store.push_back(shared_ptr<foonode>(new foonode));
  return store.back().get();
}

foonode * generate_graph()
{
  const int N=20;
  foonode *graphbegin, *rowbegin, *lastrow=NULL, *left, *current;
  int i,j;
  for (graphbegin=current=newFoonode(), i=0; i<N; i++, current=newFoonode())
    {
      if (lastrow) 
	{
	  lastrow->down=current;
	  current->up=lastrow;
	}
      for (rowbegin=current, j=1; j<N; j++)
	{
	  left=current;
	  current=newFoonode();
	  current->left=left;
	  left->right=current;
	  if (lastrow)
	    {
	      lastrow=lastrow->right;
	      current->up=lastrow;
	      lastrow->down=current;
	    }
	}
      lastrow=rowbegin;
      current->right=rowbegin;
      rowbegin->left=current;
    }
  /* toroidally connect first and last rows */
  current=graphbegin; lastrow=rowbegin;
  do 
    {
      current->up=lastrow;
      lastrow->down=current;
      current=current->right;
      lastrow=lastrow->right;
    } while (current!=graphbegin);
  return graphbegin;
}


int main()
{
  foonode *A=generate_graph(), *B;
  pack_t p;

  try 
    {
      p << A;
      p >> B;
    }
  catch (const pack_error& x)
    {
      puts(x.what()); exit(1);
    }
  
  foonode *Arow=A, *Brow=B;
  do
    {
      foonode  *Aptr=Arow, *Bptr=Brow; 
      do
	{
	  if (Aptr->nodeid!=Bptr->nodeid)
	    {
	      printf("nodeids: %d %d differ!\n",Aptr->nodeid,Bptr->nodeid);
	      exit(1);
	    }
	  if (Aptr==Bptr)
	    {
	      puts("Aptr & Bptr identical");
	      exit(1);
	    }
	  Aptr=Aptr->right;
	  Bptr=Bptr->right;
	} while (Aptr!=Arow);
      Arow=Arow->down;
      Brow=Brow->down;
    } while (Arow!=A);
  return 0;
}

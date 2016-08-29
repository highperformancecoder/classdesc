// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

// Bits to support a specialization of diffusion objects: "heat space".
// Most of the real work is done in Diffuse, which implements a CA.
// These functions simplify and stereotype access to the space variable,
// making the Heatbug code higher level.

#import <random.h>
#import "HeatSpace.h"

// global constant: maximum heat.
// This could just be used from the Diffuse2d object's max states.

const HeatValue maxHeat = 0x7fff;

@implementation HeatCell

- setX: (int)theX
{
    x = theX;
    return self;
}
- setY: (int)theY
{
    y = theY;
    return self;
}
- (int)getX
{ 
    return x;
}
- (int)getY
{
    return y;
}
@end

@implementation HeatSpace

// Add heat to the current spot. This code checks the bounds on maxHeat,
// pegs value at the top.

-addHeat: (HeatValue) moreHeat X: (int) x Y: (int) y {
  HeatValue heatHere;

  heatHere = [self getValueAtX: x Y: y];	  // read the heat
  if (moreHeat <= maxHeat - heatHere)             // would add be too big?
    heatHere = heatHere + moreHeat;		  //   no, just add
  else
    heatHere = maxHeat;				  //   yes, use max
  [self putValue: heatHere atX: x Y: y];	  // set the heat.
  return self;
}

// Search the 9 cell neighbourhood for the requested extreme (cold or hot)
// The X and Y arguments are used both as input (where to search from)
// and as output (pointers are filled with the coordinate of the extreme).
// Note that wraparound edges (boundary conditions) are implicitly in
// the code - look at the call to [self getValueAtX:Y:].

- (HeatValue) findExtremeType: (HeatExtremeType) type X: (int *) px Y: (int *) py
{
  HeatValue bestHeat;
  int x, y;
  id <List>heatList;
  id  cell, bestCell;
  int offset;

  // prime loop: assume extreme is right where we're standing
  bestHeat = [self getValueAtX: *px Y: *py];

  // Now scan through the world, finding the best cell in the 8 cell
  // nbd.  To remove the bias from the choice of location, we keep a
  // list of all best ones and then choose a random location if there
  // are points of equal best heat.
  heatList = [List create: [self getZone]];

  for (y = *py - 1; y <= *py + 1; y++) {  
      for (x = *px - 1; x <= *px + 1; x++) {
          HeatValue heatHere;
          BOOL hereIsBetter, hereIsEqual;

          heatHere = [self getValueAtX: (x+xsize)%xsize Y: (y+ysize)%ysize];
          hereIsBetter = (type == cold) ? (heatHere < bestHeat)
              : (heatHere > bestHeat);
          
          // printf("@ (%d,%d) = %d ", x, y, heatHere);
          
          hereIsEqual = (heatHere == bestHeat);
          
          if (hereIsBetter) {	      // this spot more extreme
              
              cell = [[[HeatCell create: [self getZone]] setX: x] setY: y];
              
              // this heat must be the best so far, so delete all the
              // other cells we have accumulated
              [heatList deleteAll];
              [heatList addLast: cell]; // now list only has the one new cell

              bestHeat = heatHere;   // update information
          }
          
          // if we have spots of equal best heat - then we add to the
          // list from which we can choose randomly later
          if (hereIsEqual) {
              cell = [[[HeatCell create: [self getZone]] setX: x] setY: y];
              [heatList addLast: cell]; // add to the end of the list
          }
      }
      // printf("\n");
  }
  
  // printf("---------\n");

  // choose a random position from the list
  offset = [uniformIntRand getIntegerWithMin: 0L 
                           withMax: ([heatList getCount] - 1)];

  // choose a point at random from the heat list
  bestCell = [heatList atOffset: offset];

  // Now we've found the requested extreme. Arrange to return the
  // information (normalize coordinates), and return the heat we found.

  *px = ([bestCell getX] + xsize) % xsize;
  *py = ([bestCell getY] + ysize) % ysize;

  // clean up the temporary list of (x,y) points
  [heatList deleteAll];
  [heatList drop];

  return ([self getValueAtX: *px Y: *py]);
}

@end


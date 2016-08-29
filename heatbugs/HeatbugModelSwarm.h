/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

// The Heatbug Model swarm encapsulates all the objects used in the
// simulated heatbug world itself (but not the user interface objects)

#import "Heatbug.mh"
#import "HeatSpace.h"
#import <space.h> // Grid2d
#import <objectbase/Swarm.h>

@interface HeatbugModelSwarm: Swarm
{
  int numBugs;					  // simulation parameters
  double evaporationRate;
  double diffuseConstant;
  int worldXSize, worldYSize;
  int minIdealTemp, maxIdealTemp;
  int minOutputHeat, maxOutputHeat;
  double randomMoveProbability;

  BOOL randomizeHeatbugUpdateOrder;
  id <ActionGroup> modelActions;	          // scheduling data structures
  id <Schedule> modelSchedule;
  id actionForEach;                               // for frobbing randomization

  id <List> heatbugList;			  // list of all the heatbugs
  id <Grid2d> world;				  // objects representing
  HeatSpace *heat;				  // the world
}

- getHeatbugList;				  // access methods into the
- (id <Grid2d>)getWorld;			  // model swarm. These methods
- (HeatSpace *)getHeat;                           // allow the model swarm to
						  // be observed.
- (BOOL)toggleRandomizedOrder;                    // method to toggle the
                                                  // randomization feature
- addHeatbug: (Heatbug *)bug;			  // special method for demo

+ createBegin: aZone;				  // extra methods you
- createEnd;					  // provide for Swarms
- buildObjects;
- buildActions;
- activateIn: swarmContext;

@end

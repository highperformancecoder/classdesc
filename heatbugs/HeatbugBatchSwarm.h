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

// The HeatbugBatchSwarm is an alternative to the HeatbugObserverSwarm
// It's also a toplevel swarm that directs execution of a model, but it
// operates without any graphics - just does file I/O to store data.

#import <objectbase.h>
#import "HeatbugModelSwarm.h"

@interface HeatbugBatchSwarm: Swarm
{
  int loggingFrequency;	       		  // Frequency of fileI/O

  int experimentDuration;                 // When to Stop the Sim

  id displayActions;			  // schedule data structs
  id displaySchedule;
  id stopSchedule;

  HeatbugModelSwarm *heatbugModelSwarm;	  // the Swarm we're observing

                                          // The EZGraph will be used 
  id unhappyGraph;                        // in FileI/O mode rather 
                                          // than the usual Graphics 
                                          // mode...
}

+ createBegin: aZone;
- buildObjects;
- buildActions;
- activateIn: swarmContext;
- go;

// special message on ourselves to stop running.
- stopRunning;

@end

// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

#import <simtools.h>     // initSwarm () and swarmGUIMode
#import <simtoolsgui.h>  // GUISwarm
#import "HeatbugObserverSwarm.h"
#import "HeatbugBatchSwarm.h"

// The main() function is the top-level place where everything starts.
// For a typical Swarm simulation, in main() you create a toplevel
// Swarm, let it build and activate, and set it to running.


int
main (int argc, const char **argv)
{
  id theTopLevelSwarm;

  // Swarm initialization: all Swarm apps must call this first.
  initSwarm (argc, argv);
  
  // swarmGUIMode is set in initSwarm(). It's set to be NO if you typed
  // `heatbugs --batchmode' or `heatbugs -b'. Otherwise, it's set to YES.
  if (swarmGUIMode == YES)
    {
      // We've got graphics, so make a full ObserverSwarm to get GUI objects
      theTopLevelSwarm = [HeatbugObserverSwarm createBegin: globalZone];
      SET_WINDOW_GEOMETRY_RECORD_NAME (theTopLevelSwarm);
      theTopLevelSwarm = [theTopLevelSwarm createEnd];
    }
  else
    // No graphics - make a batchmode swarm (using the key
    // `batchSwarm' from the default lispAppArchiver) and run it.
    if ((theTopLevelSwarm = [lispAppArchiver getWithZone: globalZone 
                                             key: "batchSwarm"]) == nil) 
      raiseEvent(InvalidOperation, 
                 "Can't find the parameters to create batchSwarm");

  [theTopLevelSwarm buildObjects];
  [theTopLevelSwarm buildActions];
  [theTopLevelSwarm activateIn: nil];
  [theTopLevelSwarm go];

  // theTopLevelSwarm has finished processing, so it's time to quit.
  return 0;
}

// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

#import "HeatbugObserverSwarm.h"
#import "HeatbugModelSwarm.h"
#import <collections.h>
#import <objectbase.h>
#import <analysis.h>
#import <gui.h>

@implementation HeatbugObserverSwarm

// createBegin: here we set up the default observation parameters.

+ createBegin: aZone
{
  HeatbugObserverSwarm *obj;
  id <CustomProbeMap> probeMap;
  
  // Superclass createBegin to allocate ourselves.

  obj = [super createBegin: aZone];

  // Fill in the relevant parameters (only one, in this case).

  obj->displayFrequency = 1;

  // Also, build a customized probe map. Without a probe map, the default
  // is to show all variables and messages. Here we choose to
  // customize the appearance of the probe, give a nicer interface.

  probeMap = [CustomProbeMap create: aZone
                             forClass: [self class]
                             withIdentifiers: "displayFrequency", 
                             ":", "graphBug:", NULL];
  
  // Now install our custom probeMap into the probeLibrary.

  [probeLibrary setProbeMap: probeMap For: [self class]];

  return obj;
}

// createEnd: create objects we know we'll need. In this case, none,
// but you might want to override this.

- createEnd
{
  return [super createEnd];
}

- _worldRasterDeath_ : caller
{
  [worldRaster drop];
  worldRaster = nil;
  return self;
}

- _unhappyGraphDeath_ : caller
{
  [unhappyGraph drop];
  unhappyGraph = nil;
  return self;
}

// Create the objects used in the display of the model. This code is
// fairly complicated because we build a fair number of widgets. It's
// also a good example of how to use the display code.

- buildObjects
{
  int i;

  [super buildObjects];
  
  // First, we create the model that we're actually observing. The
  // model is a subswarm of the observer. 

  // IMPORTANT!!
  // There is no need to explicitly create a Zone any longer - they
  // are `implicitly' created when you instantiate a subclass of
  // `Swarm'.

  heatbugModelSwarm = [HeatbugModelSwarm create: self];
  
  // Now create probe objects on the model and ourselves. This gives a
  // simple user interface to let the user change parameters.

  CREATE_ARCHIVED_PROBE_DISPLAY (heatbugModelSwarm);
  CREATE_ARCHIVED_PROBE_DISPLAY (self);
 
  // Instruct the control panel to wait for a button event: we halt here
  // until someone hits a control panel button so the user can get a
  // chance to fill in parameters before the simulation runs

  [controlPanel setStateStopped];

  // OK - the user has specified all the parameters for the simulation.
  // Now we're ready to start.

  // First, let the model swarm build its objects.

  [heatbugModelSwarm buildObjects];

  // Now get down to building our own display objects.

  // First, create a colormap: this is a global resource, the information
  // here is used by lots of different objects.

  colormap = [Colormap create: self];

  // Colours [0,64) are assigned to the range Red [0, 1), for heat display.

  for (i = 0; i < 64; i++)
    [colormap setColor: i ToRed: (double)i / 63.0 Green: 0 Blue: 0];

  // Colour 64 is set to green, to display heatbugs

  [colormap setColor: 64 ToName: "green"];

  // Colour 65 is set to white, used in this case below on probed heatbug.

  [colormap setColor: 65 ToName: "white"];

  // Now go in to the heatbugs in the model and set their colours to green (64)

  [[heatbugModelSwarm getHeatbugList] forEach: M(setBugColor:) : (id) 64];
  
  // Next, create a 2d window for display, set its size, zoom factor, title.
  
  worldRaster = [ZoomRaster createBegin: self];
  SET_WINDOW_GEOMETRY_RECORD_NAME (worldRaster);
  worldRaster = [worldRaster createEnd];
  [worldRaster enableDestroyNotification: self
               notificationMethod: @selector (_worldRasterDeath_:)];
  [worldRaster setColormap: colormap];
  [worldRaster setZoomFactor: 4];
  [worldRaster setWidth: [[heatbugModelSwarm getWorld] getSizeX]
	       Height: [[heatbugModelSwarm getWorld] getSizeY]];
  [worldRaster setWindowTitle: "Heat World"];
  [worldRaster pack];				  // draw the window.

  // Now create a Value2dDisplay: this is a special object that will
  // display arbitrary 2d value arrays on a given Raster widget.

  heatDisplay = 
    [Value2dDisplay create: self 
                    setDisplayWidget: worldRaster 
                    colormap: colormap
                    setDiscrete2dToDisplay: [heatbugModelSwarm getHeat]];

  [heatDisplay setDisplayMappingM: 512 C: 0];	  // turn [0,32768) -> [0,64)

  // And also create an Object2dDisplay: this object draws heatbugs on
  // the worldRaster widget for us, and also receives probes.

  heatbugDisplay = 
    [Object2dDisplay create: self
                     setDisplayWidget: worldRaster
                     setDiscrete2dToDisplay: [heatbugModelSwarm getWorld]
                     setDisplayMessage: M(drawSelfOn:)];
    
  [heatbugDisplay setObjectCollection: [heatbugModelSwarm getHeatbugList]];
 
  // Also, tell the world raster to send mouse clicks to the heatbugDisplay
  // this allows the user to right-click on the display to probe the bugs.

  [worldRaster setButton: ButtonRight
               Client: heatbugDisplay
               Message: M(makeProbeAtX:Y:)];

  // Create the graph widget to display unhappiness.

  unhappyGraph = 
    [EZGraph create: self
             setTitle: "Unhappiness of bugs vs. time"
             setAxisLabelsX: "time" Y: "unhappiness"
             setWindowGeometryRecordName: "unhappyGraph"];
  
  [unhappyGraph enableDestroyNotification: self
                notificationMethod: @selector (_unhappyGraphDeath_:)];

  [unhappyGraph createAverageSequence: "unhappiness"
                         withFeedFrom: [heatbugModelSwarm getHeatbugList] 
                          andSelector: M(getUnhappiness)];

  return self;
}  

- _update_
{
  if (worldRaster)
    {
      [heatDisplay display];
      [heatbugDisplay display];
      [worldRaster drawSelf];
#ifdef SCREENSHOTS
      {
        char filename[40];
#ifndef FULL
	
        sprintf (filename, "graph%07ld.png", getCurrentTime ());
        [actionCache doTkEvents];
        [[[[[[Pixmap createBegin: self]
              setWidget: [unhappyGraph getGraph]]
             setDecorationsFlag: NO]
            createEnd] save: filename] drop];
        sprintf (filename, "raster%07ld.png", getCurrentTime ());
        [[[[[[Pixmap createBegin: self]
              setWidget: worldRaster]
             setDecorationsFlag: YES]
            createEnd] save: filename] drop];
#else
        sprintf (filename, "screen%07ld.png", getCurrentTime ());
	[actionCache doTkEvents];
        [[[[[[Pixmap createBegin: self]
              setWidget: nil]
	     setDecorationsFlag: NO]
            createEnd] save: filename] drop];
#endif
      }
#endif
    }
  if (unhappyGraph)
    [unhappyGraph step];
  return self;
}

// Create the actions necessary for the simulation. This is where
// the schedule is built (but not run!)
// Here we create a display schedule - this is used to display the
// state of the world and check for user input. This schedule should
// be thought of as independent from the model - in particular, you
// will also want to run the model without any display.

- buildActions
{
  [super buildActions];
  
  // First, let our model swarm build its own schedule.

  [heatbugModelSwarm buildActions];
  
  // Create an ActionGroup for display: a bunch of things that occur in
  // a specific order, but at one step of simulation time. Some of these
  // actions could be executed in parallel, but we don't explicitly
  // notate that here.
  
  displayActions = [ActionGroup create: self];

  // Schedule up the methods to draw the display of the world
  // Also schedule the update of the unhappiness graph
  //  [displayActions createActionTo: unhappinessAverager message: M(update)];

  [displayActions createActionTo: self message: M(_update_)];

  // Schedule the update of the probe displays

  [displayActions createActionTo: probeDisplayManager message: M(update)];

  // Finally, schedule an update for the whole user interface code.
  // This is crucial: without this, no graphics update and the control
  // panel will be dead. It's best to put it at the end of the display schedule

  [displayActions createActionTo: actionCache message: M(doTkEvents)];

  // And the display schedule. Note the repeat interval is set from our
  // own Swarm data structure. Display is frequently the slowest part of a
  // simulation, so redrawing less frequently can be a help.
  
  // note frequency!
  displaySchedule = [Schedule create: self setRepeatInterval: displayFrequency];
  
  [displaySchedule at: 0 createAction: displayActions];
  
  return self;
}  

// activateIn: - activate the schedules so they're ready to run.
// The swarmContext argument has to do with what we were activated *in*.
// Typically the ObserverSwarm is the top-level Swarm, so it's activated
// in "nil". But other Swarms and Schedules and such will be activated
// inside of us.

- activateIn:  swarmContext
{
  // First, activate ourselves (just pass along the context).

  [super activateIn: swarmContext];

  // Activate the model swarm in ourselves. The model swarm is a
  // subswarm of the observer swarm.

  [heatbugModelSwarm activateIn: self];

  // Now activate our schedule in ourselves. This arranges for the
  // execution of the schedule we built.

  [displaySchedule activateIn: self];
  
  // Activate returns the swarm activity - the thing that's ready to run.

  return [self getActivity];
}

// You could override the "go" method here if you want something special
// to happen when the model and observer actually start running. But
// the default GUISwarm go is probably good enough.

- graphBug: aBug
{
  if (unhappyGraph)
    [unhappyGraph createSequence: "Bug" 
                  withFeedFrom: aBug 
                  andSelector: M(getUnhappiness)];
  return self;
}

@end

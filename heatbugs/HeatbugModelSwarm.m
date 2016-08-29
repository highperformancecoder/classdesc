// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

#import "HeatbugModelSwarm.h"
#import <random.h>
#import <space.h>
#import <activity.h>

@implementation HeatbugModelSwarm

// These methods provide access to the objects inside the ModelSwarm.
// These objects are the ones visible to other classes via message call.
// In theory we could just let other objects use Probes to read our state,
// but message access is frequently more convenient.

- getHeatbugList
{
  return heatbugList;
}

- (id <Grid2d>)getWorld
{
  return world;
}

- (HeatSpace *)getHeat
{
  return heat;
}

- (void)_syncUpdateOrder_
{
  if (actionForEach)
    [actionForEach setDefaultOrder:
                     (id <Symbol>) (randomizeHeatbugUpdateOrder
                                    ? Randomized
                                    : Sequential)];
}

- (BOOL)toggleRandomizedOrder
{
  randomizeHeatbugUpdateOrder = !randomizeHeatbugUpdateOrder;
  [self _syncUpdateOrder_];
  return randomizeHeatbugUpdateOrder;
}

// This method isn't normally used, but is convenient when running probes:
// it lets you easily clone a heatbug and drag it into the model.

- addHeatbug: (Heatbug *)bug
{
  [heatbugList addLast: bug];
  return self;
}

// createBegin: here we set up the default simulation parameters.

+ createBegin: aZone
{
  HeatbugModelSwarm *obj;
  id <CustomProbeMap> probeMap;

  // First, call our superclass createBegin - the return value is the
  // allocated HeatbugModelSwarm object.

  obj = [super createBegin: aZone];

  // Now fill in various simulation parameters with default values.

  obj->numBugs = 100;
  obj->evaporationRate = 0.99;
  obj->diffuseConstant = 1.0;
  obj->worldXSize = 80;
  obj->worldYSize = 80;
  obj->minIdealTemp = 17000;
  obj->maxIdealTemp = 31000;
  obj->minOutputHeat = 3000;
  obj->maxOutputHeat = 10000;
  obj->randomizeHeatbugUpdateOrder = NO;
  obj->randomMoveProbability = 0.0;

  // And build a customized probe map. Without a probe map, the default
  // is to show all variables and messages. Here we choose to
  // customize the appearance of the probe, give a nicer interface.

  // The identifier list includes all the simulation parameters in a
  // list, then (separated by the ":" delimiter) two methods
  // "toggleRandomizedOrder" and "addHeatbug" method for a probe demo.

  probeMap = 
    [CustomProbeMap create: aZone 
                    forClass: [self class]
                    withIdentifiers:  "numBugs", "diffuseConstant",
                    "worldXSize", "worldYSize", "minIdealTemp", "maxIdealTemp",
                    "minOutputHeat", "maxOutputHeat", "evaporationRate",
                    "randomMoveProbability", ":", "toggleRandomizedOrder", 
                    "addHeatbug:", NULL]; 

  // Now install our custom probeMap into the probeLibrary.

  [probeLibrary setProbeMap: probeMap For: [self class]];
  
  return obj;
}

// createEnd: we could create some objects here if we knew we needed
// them. But this method is called before the user is allowed to fill
// in any customization of the model, so we defer most object creation
// to later. (In this example, this method does nothing at all and could
// just be inherited. But it's here to show you a place to customize.)

- createEnd
{
  return [super createEnd];
}

// Now it's time to build the model objects. We use various parameters
// inside ourselves to choose how to create things.

- buildObjects
{
  int i;

  // allow our parent class to build anything.

  [super buildObjects];
  
  // First, set up objects used to represent the environment.
  // The heatspace agent represents the spatial property of heat.
  // It is initialized via various model parameters.

  heat = [HeatSpace create: self setSizeX: worldXSize Y: worldYSize
                    setDiffusionConstant: diffuseConstant
                    setEvaporationRate: evaporationRate];
  
  // Now set up the grid used to represent agent position

  world = [Grid2d create: self setSizeX: worldXSize Y: worldYSize];

  // Create a list to keep track of the heatbugs in the model.

  heatbugList = [List create: self];
  
  // Create heatbugs themselves. This is a fairly complex step, as is
  // appropriate: the heatbugs are essential aspects of the simulation.

  // First, a quick hack. During creation we might put several heatbugs
  // in the same square. This is a design flaw, but it's one that's not
  // fatal, so we ask the world object not to warn us about it. This is
  // not an example to be emulated :-)

  [world setOverwriteWarnings: 0];

  // Now a loop to create a bunch of heatbugs.

  for (i = 0; i < numBugs; i++)
    {
      Heatbug * hbug;
      int idealTemp, outputHeat;
      
      // Choose a random ideal temperature, output heat from the specified
      // range (model parameters).
      
      idealTemp = [uniformIntRand
                    getIntegerWithMin: minIdealTemp withMax: maxIdealTemp];
      outputHeat = [uniformIntRand
                     getIntegerWithMin: minOutputHeat withMax: maxOutputHeat];
      
      
      // Create the heatbug, set the creation time variables
      
      hbug = [Heatbug createBegin: self];
      [hbug setWorld: world Heat: heat];
      hbug = [hbug createEnd];
      hbug->hbID = i;
      // Add the bug to the end of the list.
      
      [heatbugList addLast: hbug];
      
      // Now initialize the rest of the heatbug's state.
      
      [hbug setIdealTemperature: idealTemp];
      [hbug setOutputHeat: outputHeat];
      [hbug setRandomMoveProbability: randomMoveProbability];
      
      [hbug setX: [uniformIntRand
                    getIntegerWithMin: 0L
                    withMax: (worldXSize-1)]  // random position
            Y: [uniformIntRand getIntegerWithMin: 0L withMax: (worldYSize-1)]];
    }
  [world setOverwriteWarnings: 1];		  // ok, done cheating.
  
  return self;
}

// Here is where the model schedule is built, the data structures
// that define the simulation of time in the mode. The core is an
// actionGroup that has a list of actions. That's then put in a Schedule.

- buildActions
{
  [super buildActions];
  
  // Create the list of simulation actions. We put these in an action
  // group, because we want these actions to be executed in a specific
  // order, but these steps should take no (simulated) time. The
  // M(foo) means "The message called <foo>". You can send a message
  // To a particular object, or ForEach object in a collection.

  // Note we update the heatspace in two phases: first run diffusion,
  // then run "updateWorld" to actually enact the changes the heatbugs
  // have made. The ordering here is significant!

  // Note also, that with the additional `randomizeHeatbugUpdateOrder'
  // Boolean flag we can randomize the order in which the bugs
  // actually run their step rule.  This has the effect of removing
  // any systematic bias in the iteration throught the heatbug list
  // from timestep to timestep

  // By default, all `createActionForEach' modelActions have a default
  // order of `Sequential', which means that the order of iteration
  // through the `heatbugList' will be identical (assuming the list
  // order is not changed indirectly by some other process).

  modelActions = [ActionGroup create: self];
  [modelActions createActionTo:      heat        message: M(stepRule)];
#ifdef FAST
  {
    id call =
      [FCall create: self
             target: [heatbugList getFirst]
             selector: M(step)
             arguments:
               [[[FArguments createBegin: self]
                  setSelector: M(step)]
                 createEnd]];
    
    actionForEach = 
      [modelActions createFActionForEachHomogeneous: heatbugList call: call];
  }
#else
  actionForEach =
    [modelActions createActionForEach: heatbugList message: M(step)];
#endif
  [self _syncUpdateOrder_];
  [modelActions createActionTo:      heat        message: M(updateLattice)];

  // Then we create a schedule that executes the modelActions. modelActions
  // is an ActionGroup, by itself it has no notion of time. In order to
  // have it executed in time, we create a Schedule that says to use
  // the modelActions ActionGroup at particular times.
  // This schedule has a repeat interval of 1, it will loop every time step.
  // The action is executed at time 0 relative to the beginning of the loop.

  // This is a simple schedule, with only one action that is just
  // repeated every time. See mousetraps for more complicated schedules.
  
  modelSchedule = [Schedule create: self setRepeatInterval: 1];
  [modelSchedule at: 0 createAction: modelActions];

  return self;
}

// Now set up the model's activation. swarmContext indicates where
// we're being started in - typically, this model is run as a subswarm
// of an observer swarm.

- activateIn: swarmContext
{
  // First, activate ourselves via the superclass activateIn: method.
  // Just pass along the context: the activity library does the right thing.

  [super activateIn: swarmContext];

  // Now activate our own schedule.

  [modelSchedule activateIn: self];

  // Finally, return our activity.

  return [self getActivity];
}

@end

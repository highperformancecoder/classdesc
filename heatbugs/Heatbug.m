// Heatbugs application. Copyright © 1996-2000 Swarm Development Group.
// This library is distributed without any warranty; without even the
// implied warranty of merchantability or fitness for a particular purpose.
// See file LICENSE for details and terms of copying.

// Heatbugs are agents in a 2d world with simple behaviour:
//   if too cold, move to warmer spot
//   if too warm, move to cooler spot.
// and some occasional exceptions
//   if the spot is occupied, try to move to an unoccupied spot.
//   randomMoveProbability chance of moving to a random spot

#import "Heatbug.mh"
#import <random.h>

#define USE_PIXMAP
#ifdef USE_PIXMAP
static id <Pixmap> bugPixmap = nil;
#endif

// Defining the methods for a Heatbug.

@implementation Heatbug

- init
{ [super init];
  return self;
}

void cpp_Heatbug_prtvars(Heatbug * obj);
- prtvars
{ cpp_Heatbug_prtvars(self); return self; }

// Initialize crucial state for the heatbug.

- setWorld: (id <Grid2d>)w Heat: (HeatSpace *)h
{
  // Strictly speaking, this check isn't necessary. But we intend these
  // parameters to be immutable once set, so to be extrasafe we check:
  // it could catch an error later.

  if (world != nil || heat != nil)
    {
      [InvalidArgument
        raiseEvent:
          "You can only set the world/heat of a heatbug at creation time\n"];
    }
  world = w;
  heat = h;

  return self;
}

// createEnd. This is a good place to put code that does various sorts
// of initialization that can only be done after some parameters of the
// object are set. It's also a good time to check for errors in creation.

- createEnd
{
  // make sure the user set up world and heat.

  if (world == nil || heat == nil)
    [InvalidCombination
      raiseEvent: "Heatbug was created without a world or heat.\n"];
  
  // Cache the worldSize for speed of later access. Note how we do
  // this in createEnd - it could also have been done when setWorld:Heat:
  // was called, but this is a good place to do it, too. If an object
  // needed to allocate extra memory, this is the right place to do it.

  worldXSize = [world getSizeX];
  worldYSize = [world getSizeY];

  // Someday, it'd be good if the space library to be powerful enough that
  // the heatbugs never need to be aware how big their world is.

  return self;					  // CRUCIAL!
}


// Methods for reading/writing a Heatbug's state during runtime.
// The probe mechanism is the lowlevel way of getting at an object's
// state - you're also allowed (but not required) to write methods to
// access the state as you find it is necessary or convenient.
// Note the naming convention: for a variable named "fooBar" methods are
//   -(sometype) getFooBar;
//   -setFooBar;
// this naming convention will be important for a later version of probe.
// (probe will preferentially use these methods instead of direct access).


// Reading unhappiness is a common enough operation that we provide a
// special method.

- (double)getUnhappiness
{
  return unhappiness;
}

// Simple set methods for Heatbug state. Some of these are probably not
// going to normally change in a heatbugs lifetime, but there's no reason
// they couldn't change.

- setIdealTemperature: (HeatValue)i
{
  idealTemperature = i;
  return self;
}

- setOutputHeat: (HeatValue)o
{
  outputHeat = o;
  return self;
}

- setRandomMoveProbability: (float)p
{
  randomMoveProbability = (float) p;
  return self;
}

// This method is a bit dangerous: we blindly put ourselves on top of
// the grid no matter what's underneath us: because Grid2d only allows
// one object per square, we could be destroying data. This is poor
// design, but fortunately doesn't kill us in this particular app. If
// some other object really needed to find all objects based on
// looking in the grid, it would cause problems. (But note, in heatbug
// creation, how we tell Grid2d to turn off its warnings about overwrites)

- setX: (int)inX Y: (int)inY
{
  x = inX;
  y = inY;
  [world putObject: self atX: x Y: y];		  // yikes!
  return self;
}


// All of the previous code is basic Swarm object programming. The
// real simulation code follows.


// Heatbug behaviour is actually implemented here. The notion of a "step"
// method is a nice simplification for basic simulations.
//void objc_worldMoveObject(void * worldobj, int oldx, int oldy, int newx, int newy);

void cpp_Heatbug_step(Heatbug * obj);
- step
{ cpp_Heatbug_step(self);  return self; }


// Extra bits of display code: setting our colour, drawing on a window.
// This code works, but it'd be better if there were a generic object
// that knew how to draw agents on grids.

- setBugColor: (Color)c
{
  bugColor = c;
  return self;
}

#ifdef USE_PIXMAP
- _getPixmap_: (id <Raster>)r
{
  if (bugPixmap == nil)
    {
      bugPixmap = [Pixmap createBegin: [self getZone]];
      
      [bugPixmap setDirectory: [arguments getAppDataPath]];
      [bugPixmap setFile: "ant.png"];
      bugPixmap = [bugPixmap createEnd];
      [bugPixmap setRaster: r];
    }
  
  return bugPixmap;
}
#endif

- drawSelfOn: (id <Raster>)r
{
#ifndef USE_PIXMAP
  [r drawPointX: x Y: y Color: bugColor];
#else
  [r draw: [self _getPixmap_: r] X: x Y: y];
#endif

  return self;
}

@end

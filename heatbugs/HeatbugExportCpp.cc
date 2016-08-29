/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <stdarg.h>

// C++ interface function to ObjC

// method implemented in C++
// - for ObjC to call C++ functions

extern "C" void cpp_Heatbug_prtvars(Heatbug * obj)
{ obj->prtvars(); }

extern "C" void cpp_Heatbug_step(Heatbug * obj)
{ obj->step(); }

// -----------------------------------------------------------------------

// methods implemented in ObjC
// - for C++ to call ObjC functions

// Swarm related objects

extern "C" long int objc_uniformIntRand(long int min, long int max);
extern "C" double   objc_uniformDblRand(double min, double max);

extern "C" void *   objc_worldGetObject(void * worldobj, int px, int py);
extern "C" void     objc_worldPutObject(void * worldobj, void * obj, int px, int py);
extern "C" void     objc_worldMoveObject(void * worldobj, int oldx, int oldy, int newx, int newy);

// Heatbugs related

extern "C" int      objc_getHeat(void * heatobj, int px, int py);
extern "C" void     objc_addHeat(void * heatobj, int hval, int px, int py);
extern "C" int      objc_findExtremeType(void * heatobj, HeatExtremeType ht, int * px, int * py);


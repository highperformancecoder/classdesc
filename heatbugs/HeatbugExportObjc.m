#include <stdarg.h>
#import <random.h>
#import "Heatbug.mh"
//#include <stdio.h>

extern long int objc_uniformIntRand(long int min, long int max)
{ return [uniformIntRand getIntegerWithMin: min withMax: max]; }

extern double objc_uniformDblRand(double min, double max)
{ return [uniformDblRand getDoubleWithMin: min withMax: max]; }

extern int objc_getHeat(void * heatobj, int px, int py)
{ return [(HeatSpace *)heatobj getValueAtX: px Y: py]; }

extern void objc_addHeat(void * heatobj, int hval, int px, int py)
{ [(HeatSpace *)heatobj addHeat: hval X: px Y: py]; }

extern void objc_findExtremeType(void * heatobj, HeatExtremeType ht, int * px, int * py)
{ [(HeatSpace *)heatobj findExtremeType: ht X: px Y: py]; }

extern id objc_worldGetObject(void * worldobj, int px, int py)
{ return [ (id <Grid2d>)worldobj getObjectAtX: px Y: py]; }

extern void objc_worldPutObject(void * worldobj, void * obj, int px, int py)
{ [ (id <Grid2d>)worldobj putObject: (id)obj atX: px Y: py]; }

// move the heatbug object from (oldx,oldy) to new location (newx,newy)
extern void objc_worldMoveObject(void * worldobj, int oldx, int oldy, int newx, int newy)
{  Heatbug * hb;
   hb = (Heatbug *) [ (id <Grid2d>) worldobj getObjectAtX: oldx Y: oldy];
   [(id <Grid2d>)worldobj putObject: nil atX: oldx Y: oldy];
   [(id <Grid2d>)worldobj putObject: hb  atX: newx Y: newy];
   hb->x = newx;
   hb->y = newy;
   //printf("t=%d hb%d moves (%d,%d) to (%d,%d)\n",hb->time,hb->hbID,oldx,oldy,hb->x,hb->y);
}

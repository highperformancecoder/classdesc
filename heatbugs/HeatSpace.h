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

// The HeatSpace is a simple object to represent the heat in the
// world: a spatial variable. HeatSpace inherits most of its behaviour
// from the "Diffuse" space Object.

#import <space/Diffuse2d.h>
#import <space.h>

// An abstract type for values of heat.
typedef int HeatValue;

// The maximum heatvalue a square can use
extern const HeatValue maxHeat;

// Enum used in findExtremeType:X:Y:
typedef enum { cold, hot } HeatExtremeType;

// Class HeatCell. Stores an (x,y) co-ordinate
@interface HeatCell: SwarmObject 
{
    int x;
    int y;
}
- setX: (int)theX;
- setY: (int)theY;
- (int)getX;
- (int)getY;
@end

// Class HeatSpace. Inherit from Diffuse, don't add any new variables
@interface HeatSpace: Diffuse2d
{
}

// New method: add heat to a specific square  
- addHeat: (HeatValue)moreHeat X: (int)x Y: (int)y;

// New method: search a neighbourhood for the requested extreme. 
- (HeatValue)findExtremeType: (HeatExtremeType)heat X: (int *)x Y: (int *)y;
@end


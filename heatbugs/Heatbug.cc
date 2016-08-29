/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <iostream>
#include "Heatbug.h"
#include <stdio.h>

#ifdef OBJC
#include "HeatbugExportCpp.cc"
#endif

using namespace std;

void Heatbug::prtvars()
{
   int newX = x;
   int newY = y;
   cout << "Hb2=" << sizeof(Heatbug);
   cout << " ID=" << hbID;
   cout << " uh=" << unhappiness;
   cout << " Xp=" << x;
   cout << " Yp=" << y;
   cout << " C=" << (int) bugColor;

   cout << " Xw=" << worldXSize;
   cout << " Yw=" << worldYSize;
   cout << " Ti=" << idealTemperature;
   cout << " To=" << outputHeat;

   int heatHere = objc_getHeat(heat,x,y);
   HeatExtremeType ht = (heatHere < idealTemperature) ? hot : cold;
   objc_findExtremeType(heat, ht, &newX, &newY);

   cout << " ##=" << newX << " " << newY;
//   cout << " Pr=" << randomMoveProbability;
   cout << "\n";
}

// C++ implementation of the Heatbug's step method
// pls refer to original ObjC implementation for comparation
//
void Heatbug::step()
{  HeatValue        heatHere;          // heat at curr loc
   int              newX;              // next loc to move to
   int              newY;
   int              tries;
   HeatExtremeType  ht;                // next loc ExtremeHeatType

   heatHere = objc_getHeat(heat,x,y);  // heat at curr pos
   time += 1;                          // simulation time
   if (heatHere < idealTemperature)    // calc unhappiness
     unhappiness = (double)(idealTemperature - heatHere) / maxHeat;
   else
     unhappiness = (double)(heatHere - idealTemperature) / maxHeat;

   newX = x; newY = y;
   ht = (heatHere < idealTemperature) ? hot : cold;   // next loc heat type
   objc_findExtremeType(heat, ht, &newX, &newY);      // warmest/coldest spot

   if ( ((float) objc_uniformDblRand(0.0, 1.0)) < randomMoveProbability )
   {  newX = (x + objc_uniformIntRand(-1L, 1L));      // pick a random spot
      newY = (y + objc_uniformIntRand(-1L, 1L));
      newX = (newX + worldXSize) % worldXSize;        // normalize coords
      newY = (newY + worldYSize) % worldYSize;
   }
   cout << "T=" << time << " Hb" << hbID << " at (" << x << "," << y << ") feels ";
   if (unhappiness == 0)     // heatbug happy - don't move, just output heat
   {  cout << "happy\n";
      objc_addHeat(heat,outputHeat,x,y);
   }
   else
   {  tries = 0;
      if (ht==cold) cout << "hot"; else cout << "cold";
      // if heatbug happy                      - don't search
      // if heatbug unhappy & opt loc empty    - dont't search
      // if heatbug unhappy & opt loc occupied - search neighbours
      if ( (newX != x || newY != y) )
      {  while ( (objc_worldGetObject(world,newX,newY) != NULL) && (tries < 10) )
         {  int location, xm1, xp1, ym1, yp1;
            // choose randomly from the nine possible
            // random locations to move to
            location = objc_uniformIntRand(1L, 8L);
            xm1 = (x + worldXSize - 1) % worldXSize;
            xp1 = (x + 1) % worldXSize;
            ym1 = (y + worldYSize - 1) % worldYSize;
            yp1 = (y + 1) % worldYSize;
            switch (location)
            {  case 1:  newX = xm1; newY = ym1; break;  // NW
               case 2:  newX = x  ; newY = ym1; break;  // N
               case 3:  newX = xp1; newY = ym1; break;  // NE
               case 4:  newX = xm1; newY = y;   break;  // W
               case 5:  newX = xp1; newY = y;   break;  // E
               case 6:  newX = xm1; newY = yp1; break;  // SW
               case 7:  newX = x  ; newY = yp1; break;  // S
               case 8:  newX = xp1; newY = yp1;         // SE
               default: break;
            }
            tries++;              // don't try too hard.
         }                        // while < 10 tries, continue search
         if (tries == 10)         // No nearby clear spot, so don't move.
         {  newX = x; newY = y;
         }
      }
      objc_addHeat(heat, outputHeat, x, y);      // output heat before move
      if   ((x==newX) && (y==newY)) cout << " no move\n";
      else cout << " moves to (" << newX << "," << newY << ")\n";
      objc_worldMoveObject(world, x, y, newX, newY);  // move to new loc
   }
}


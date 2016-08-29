/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// C++ Heatbug class
//
#include <stdarg.h>
#include "../objc-examples/ObjCsupport.h"          // ObjC supports

// Heatbugs related type definitions
typedef int HeatValue;
typedef unsigned char Color;
typedef enum {cold,hot} HeatExtremeType;
extern const HeatValue maxHeat;
//const HeatValue maxHeat = 0x7fff;

class Heatbug: public objc_obj    // need to be derived from ObjC class
{
   public:
      unsigned    zbits;          //inherits fr SwarmObject/Object_s - obj allocated zone
      int         hbID;
      int         time;
      double      unhappiness;
      int         x;
      int         y;
      HeatValue   idealTemperature;
      HeatValue   outputHeat;
      float       randomMoveProbability;
      id          world;
      int         worldXSize;
      int         worldYSize;
      id          heat;
      Color       bugColor;

   public:
      void prtvars();
      void step();

};

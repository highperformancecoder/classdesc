/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

#include <iostream>
#include "myCounter.h"
#include <stdio.h>

using namespace std;

int myCounter::prtvars()
{
   cout << "MyCounter C++ Object: byte=<" << sizeof(myCounter) << ">";
   cout << " ObjName=<" << sName << "> dVal=<" << dVal << "> iInc=<" << iInc << ">\n";
   cout << "                      iaX[2][4]=<";
   for(int i=0; i<2; i++)
   {  for (int j=0; j<4; j++)
         cout << iaX[i][j] << " ";
   }
   cout << ">\n\n";
   return 22;

}

int myCounter::prtVec(vector<int> v)
{   return (int) 1; }

int myCounter::cpp_prtVec()
{   return (int) 1; }

void myCounter::next(int x)
{  dVal += (double) iInc;
}

double myCounter::sumN_x1(double x1, objc_t& buf)
{
   double x2, x3, x4, x5;
   buf >> x2 >> x3 >> x4 >> x5;
   return x1 + x2 + x3 + x4 + x5;
}

double myCounter::sum5_x1(double x1, double x2, double x3, double x4, double x5)
{  return x1 + x2 + x3 + x4 + x5; }

unsigned short int myCounter::fn2arg(unsigned short int x1, double x2)
{  return (unsigned short int) 1; }

int myCounter::fn3arg(int x1, double x2, unsigned short int x3)
{   return (int) 1; }


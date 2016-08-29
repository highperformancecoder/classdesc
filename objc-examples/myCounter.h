/*
  @copyright Russell Standish 2000-2013
  @author Russell Standish
  This file is part of Classdesc

  Open source licensed under the MIT license. See LICENSE for details.
*/

// Steps for running C++ model under ObjC environment:
// a. write your C++ model/class      (.h  file)
// b. write your model implementation (.cc file)
// c. do C++->ObjC parsing
// d. write your main ObjC program that utilised your C++ class
// e. compile and link to create final executable

#include <stdarg.h>
#include "ObjCsupport.h"            // ObjC supports
#include "vector"

class myCounter: public objc_obj {  // need to be derived from ObjC class

   public:
      char   sName[20];
      int    iaX[2][4];
      int    iInc;
      double dVal;

   public:

      // std parameter passing
      void next(int x);
      double sum5_x1(double x1, double x2, double x3, double x4, double x5);
      int prtvars();
      unsigned short int fn2arg(unsigned short int x1, double x2);
      int fn3arg(int x1, double x2, unsigned short int x3);

      // parameter passing using va_list - 1st arg must be known
      double sumN_x1(double x1, objc_t& buf);

      int prtVec(vector<int> v);   // for c++ only, no objc translation
      int cpp_prtVec();            // for C++ only, no objc translation

};

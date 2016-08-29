Objective-C/C++ interface


1.0 Introduction

    Purpose:

    - Running a C++ model in ObjC (Swarm) environment

    Aim:

    - A translation program that will automatically generated
      the necesary interface files from the give C++ header
      file.

    - When these interface files are compiled and linked with
      the user's C++ model, a final executable application program
      will be created.

    - This application program can either be run in ObjC or C++
      environment and will be able to access the class defined
      from both environment.

2.0 Approach

    - Given:

         myCounter.h             <-- C++ class definition file
         myCounter.cc            <-- C++ implemntation file
         main.m                  <-- Objective-C main application prgm

    - We need to create 3 interface files:

         myCounter.mh            <-- Objective-C class (template)
         myCounter.m             <-- Objective-C interface
         myCounterExportCpp.cc   <-- C++/ObjC interface

    - we are using a modified "classdesc" to parse the myCounter.h
      file and output myCounter.cd file (which contains serialization
      information about the class).

    - Include this "myCounter.cd" file with a small stub C++ program
      (write_objc.cc) to produce an executable.

         input:      myCounter.cd       <-- produced by Classdesc
                     myCounter.h        <-- C++ class
                     myCounter.cc       <-- and its implementation
                     write_objc.cc      <-- small C++ stub prgm
                     writeobjc_base.h   <-- required by Classdesc

         executable: write_objc

    - Then run this executable (translator) to produce the required
      three interface files as shown above.

    - final executable consists of following Object-code files:

      Object-code   Compiled from following source(s)
      ------------  ---------------------------------------------
      myCounter.o   myCounter.h, myCounter.cc, myCounterExport.cc
      myCounter.mo  myCounter.mh, myCounter.m
      main.o        main.m

    - Finally links these objec-code files together to form an ObjC
      (OR C++) executable file.

3.0  C++ class parsing using "Classdesc"

     - classdesc modification

       * add a "-objc" switch which enable the classdesc to output
         specific ObjC related overload fuction

       * re-write a "pack_base.h" file (consists of the basic type
         serialization overload functions) and rename it to
         "writeobjc_base.h" to output specific ObjC type/method
         translation.

     - classdesc parsing/output: myCounter.cd file

       ../classdesc -objc writeobjc < myCounter.h > myCounter.cd

       #include "writeobjc_base.h"
       inline void writeobjc(writeobjc_t* targ, eco_string desc,
                             class myCounter& arg)
       {
       writeobjc(targ,desc+".isa",arg.isa);
       writeobjc(targ,desc+".sName",is_array(),arg.sName[0],"[20]");
       writeobjc(targ,desc+".iaX",is_array(),arg.iaX[0][0],"[2][4]");
       writeobjc(targ,desc+".iInc",arg.iInc);
       writeobjc(targ,desc+".dVal",arg.dVal);
       writeobjc(targ,desc+".next",arg,&myCounter::next);
       writeobjc(targ,desc+".sum",arg,&myCounter::sum);
       writeobjc(targ,desc+".prtvars",arg,&myCounter::prtvars);
       }

     - ObjC auto-translator executable: write_objc

       When the myCOunter.cd file combined with myCounter.h,
       myCounter.cc, write_objc.cc and writeobjc_base.h files
       to produce an executable called an "ObjC-translator"

       $ g++ -g -c myCounter.cc
       $ g++ -g -DCNAME=myCounter -o write_objc myCounter.o write_objc.cc
       $ rm myCounter.o
       $ write_objc

     - write_objc.cc: a small "stub" program for the auto-translator

          #include "writeobjc_base.h"    // ObjC base classdesc file
          #include FILE_NAME(CNAME,.h)   // C++ header file
          #include FILE_NAME(CNAME,.cd)  // obj description file
          WRITE_OBJC_MAIN(CNAME)         // auto-translator main prgm/macro

       To compile this program, a macro "CNAME" needed to be defined
       in the g++ command line (the name of the C++ class to be compiled
       so that proper include files can be included by the compiler).

       $ g++ -g -DCNAME=myCounter -o write_objc myCounter.o write_objc.cc

     - ObjC/C++ interface files

       WHen this "ObjC-translator" executed, it will produce three
       ObjC/C++ files:

          myCounter.mh         <-- ObjC class
          myCounter.m          <-- ObjC implementation
          myCounterExport.cc   <-- C++/ObjC interface

       The myCounter.mh and myCounter.m files are the necessary ObjC class
       definition and implementation files in ObjC environment. The ObjC
       class implementation uses the ObjC/C++ interface functions as
       defined in the myCounterExport.cc file (to handle method's
       argument passing and method binding).

       The myCounterExport.cc interface file mostly consist of the C++
       exported class method that can be callable in the ObjC environment.

       An example of a C++ exported function:

          extern "C" void cpp_myCounter_sum(myCounter * obj, void * rtn, va_list * ap)
          { obj->sum(rtn, ap); }

       that is used to implementation the "sum" member method:

          void cpp_myCounter_sum(void * obj, void * rtn, void * ap);
          - sum: (void *) rtn, ...
          { va_list ap;
            va_start(ap, rtn);
            cpp_myCounter_sum(self, rtn, &ap);
            va_end(ap);
            return self;
          }


4.0  Running the example:

     - change to the objc-examples sub-directory
     - then execute following commands:

       $ make clean
       $ make

       ../classdesc -objc writeobjc < myCounter.h > myCounter.cd
       /usr/local/gcc2/bin/g++ -g -c myCounter.cc
       /usr/local/gcc2/bin/g++ -g -DCNAME=myCounter -o write_objc myCounter.o write_objc.cc
       rm myCounter.o
       write_objc

       C++ to ObjC parsing ...
       Starts parsing C++ class to ObjC ...
       Translating array: "sName[20]" - Translating simple data type: "sName" of-type "char"Translating array: "iaX[2][4]" - Translating simple data type: "iaX" of-type "int"Translating simple data type: "iInc" of-type "int"
       Translating simple data type: "dVal" of-type "double"
       Translating function: void  next(int x )
       Translating function: double  sum5_x1(double x1 , double x2 , double x3 , double x4 , double x5 )
       Translating function: int  prtvars()
       Translating function: unsigned short int  fn2arg(unsigned short int x1 , double x2 )
       Translating function: int  fn3arg(int x1 , double x2 , unsigned short int x3 )
       Translating function: double  sumN_x1(double x1 , objc_t & buf )
       Translating function: int  prtVec(vector < int > v )
       ** warning: C++ function detected, no ObjC translation.

       Translating function: int  cpp_prtVec()
       ** warning: C++ function detected, no ObjC translation.

       End translation.

       /usr/local/gcc2/bin/gcc -c -Wno-import -g main.m
       /usr/local/gcc2/bin/gcc -c -o myCounter.mo -Wno-import -g myCounter.m
       /usr/local/gcc2/bin/g++ -g -c -DOBJC   myCounter.cc
       /usr/local/gcc2/bin/g++ -g -o main main.o myCounter.mo myCounter.o -L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/2.95/ -L/usr/local/lib/libstdc++.so -lgcc -lobjc

       $ main          <-- Application which run in ObjC environment
                           and manipulates the C++ object.

       C++ -> ObjC interface testing:

       MyCounter C++ Object: byte=<68> ObjName=<c1> dVal=<5.5> iInc=<3>
                             iaX[2][4]=<10 11 12 13 20 21 22 23 >

       MyCounter C++ Object: byte=<68> ObjName=<c1> dVal=<8.5> iInc=<3>
                             iaX[2][4]=<10 11 12 13 20 21 22 23 >

       Parameter passing using va_list:   Sum = 16.500000
       Normal ObjC parameter passing:     Sum = 16.500000

       End testing.

       $

5.0  ObjC/C++ class parsing

     - Given a C++ class as shown in 5.01

     - Auto translate it to ObjC equivalent as shown in 5.02,
       so that program from either C++ and/or ObjC modules will be
       able to access the class's variables/methods.

     - The final application program (executable) consists of various
       C++ and ObjC modules (plus some interfacing modules)

     - following C++ member function will not be parse to ObjC equivalent:
       (the advantage is that user can defined and implemented C++ only
        function without worry that it will be parsed by ObjC parser).

       ** argument that contain c++ only syntax

          eg. int sumVec( vector<int> v);

       ** function name with "cpp_" string embedded

          eg: int cpp_Sum(int x)

     - Advantages:

       ** Access some of the C++ capabilities from with the
          Objective-C environment.

       ** Access some of the Objective-C classes from within the
          C++ environment.

       ** C++/ObjC modules are accessing the same object/class
          stored in the memory.

5.01 C++ class:

     class myCounter: public objc_obj    // need to be derived from ObjC class
     {  public:
           char   sName[20];
           int    iaX[2][4];
           int    iX;
           double dVal;

        public:
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

5.02 ObjC class:

     #import <objc/Object.h>
     @interface myCounter : Object
        {  @public
              char   sName[20];
              int    iaX[2][4];
              int    iInc;
              double dVal;
        }
        - init;
        - next: (int) x;
        - (double) sum5_x1: (double) x1 x2: (double) x2 x3: (double) x3 x4: (double) x4 x5: (double) x5;
        - (int) prtvars;
        - (unsigned short int) fn2arg: (unsigned short int) x1 x2: (double) x2;
        - (int) fn3arg: (int) x1 x2: (double) x2 x3: (unsigned short int) x3;
        - (double) sumN_x1: (double) x1, ...;
     @end

6. ObjC/C++ communication


6.01 The ObjC root class: "Object"

     - in the current implementation:

       * the shared objects (classes) are created in the ObjC
         environment so that they can be inherited from the
         ObjC "Object" class

       * so that they can inherite the ObjC "Object" class

       * the created objects can be manipulated from within the
         ObjC/C++ environment.

       In ObjC environment, all classes must derived from
       the root "Object" class - it defines the basic framework
       for ObjC objects and object interactions.
       It imparts to the classes and instances that inherit
       from it the ability to behave as Objects and cooperate
       with the run-time system.

6.02 Object accessing and parameter passing (pls refer to the
     example for details)

   (a) ObjC environment: to access the object's instance variables

       myCounter * c1=[[myCounter alloc] init];  <-- create an instance
       ...
       c1->dVal = 5.5;           <-- modify instance-varible dVal value
       ...

   (b) ObjC/C++ normal function argument passing

       ObjC call:

          [obj1 method: arg1 arg2lable: arg2 ... argnlabel: argn]

       equivalnet C++ call:

          method(arg1, arg2, ..., argn)

   (b) ObjC/C++ environment: using C va_list structure for arbitrary number
       of arguments for (the firrst argument must be known).

       ObjC call:

          [obj1 method: arg1, arg2, ..., argn];

       equivalnet C++ call:

          method(arg1, objc_t& buf)

       In C++, a class "objc_t" has been defined to facilitate the ease of
       extracting input arguments (arg2, ..., argn) from the method
       - just use the ">>" operator to extract arguments into variable.

       for example:  double x2, x3, x4, x5;          ;; assume total 5 args
                     buf >> x2 >> x3 >> x4 >> x5;

6.02 Normal parameter passing implement details


         ** myCounter.h file: user model
               ...
               double sum3_x1(double x1, double x2, double x3);
               ...

         ** myCounter.cc file:  user implemented model
               ...
               double myCounter::sum3_x1(double x1, double x2, double x3)
               {  return x1 + x2 + x3; }
               ...

         ** myCounter.mh file: auto-generated interface file 1
               ...
               - (double) sum3_x1: (double) x1 x2: (double) x1  x3: (double) x3;
               ...


         ** myCounter.m file: auto-generated interface file 2
               ...
               double cpp_myCounter_sum(double x1, double x2, double x3);
               - (double) sum3_x1: (double) x1 x2: (double) x1  x3: (double) x3
               { return cpp_myCounter_sum3_x1(self, x1, x2, x3); }
               ...

         ** myCounterExportCpp.cc file: ** auto-generated interface file 3
               ...
               extern "C" double cpp_myCounter_sum3_x1(myCounter * obj, double x1, double x2, double x3);
               { return obj->sum(x1, x2, x3); }
               ...

6.03 Parameter passing using va_list: implement details


         ** myCounter.h file: user model
               ...
               double sum3(double x1, objc_t& buf);
               ...

         ** myCounter.cc file:  user implemented model
               ...
               double myCounter::sum3(double x1,  objc_t& buf)
               {  double x2, x3;
                  buf >> x2 >> x3;
                  return x1 + x2 + x3;
               }
               ...

         ** myCounter.mh file: auto-generated interface file 1
               ...
               - (double) sum3: (double) x1, ... ;
               ...


         ** myCounter.m file: auto-generated interface file 2
               ...
               double cpp_myCounter_sum3(myCounter * obj, double x1, va_list * ap);
               - (double) sum3: (double) x1, ...
               { double rtnvalue;
                 va_list ap;
                 va_start(ap, x1);
                 rtnvalue = cpp_myCounter_sum3(self, x1, &ap);
                 va_end(ap);
                 return rtnvalue;
               }
               ...


         ** myCounterExportCpp.cc file: ** auto-generated interface file 3
               ...

               extern "C" double cpp_myCounter_sumN(myCounter * obj, double x1, va_list * ap)
               { objc_t buffer;
                 buffer.ap = ap;
                 return obj->sumN(x1, buffer);
               }
               ...

7. Data types supported

   - Simple data types that are supported in the C++/ObjC class
     parsing (and its multi-dimensional form):

        int                      char
        short int                unsigned char
        unsigned int             signed char
        unsigned short int       wchar_t
        long int
        unsigned long int
        float
        double
        long double

   - "bool" is not supported in ObjC

   - since the class's methods are implemented using C++ (eg. in
     myCounter.cc file), you may use whatever objects/classes/STL
     (that are supported by the C++ language) in the method
     implementation.


ObjC/C++ interface


00  To run the example:

    - pls refer to ../objc-examples/ObjC-Cpp-project-ALife8-slides.ps
      on how to modify the Swarm environment to run the modified
      heatbugs example.

01  History:

    Please refer to the Classdesc ObjC/C++ project for details.
    In that project, we defined a C++ class and it implementation
    using C++ language. We then use a modified Classdesc to parse
    the C++ class to translate it into an equivalent ObjC class
    so that we can include the translated class (which implemented
    in C++) in the ObjC main program/environment.

02  purpose:

    To modify an existing ObjC headbugs example so that the
    agent-based part can be seperated out and run in the
    C++ environment.

    The advantage is that we can implement the agent's behaviour
    using C++ (s more modern and capable OO language as compared
    to the ObjC language) while at the same time, using all the
    available tools that have already been developped int he
    Swarm environment (implemented in ObjC).

03  Approach

    - please refer to Section 10.0 on how to install the Swarm
      v2.1.1 (using gcc version 2.95).

    - please refer to Section 4.0 on how to modify the Swarm
      environment so that it can compile the modified heatbugs'
      ObjC and C++ source files (into respective object files)
      and how to incorporated the overall object files into final
      executable file.


04  Heatbugs source file modification/creation

    Convention:

       Given a class that have been defined in both the ObjC
       and C++ environment (using same class name, eg, Heatbug).
       We will use following file extentions for storing:

          Heatbug.mh file     - ObjC header file
          Heatbug.m  file     - ObjC implementation
          Heatbug.h  file     - C++ header file     (interface file 1)
          Heatbug.cc file     - C++ implementation  (interface file 2)
          HeatbugExportCpp.cc - C++->ObjC export    (interface file 3)
          HeatbugExportObjC.m - ObjC->C++ export    (interface file 4)

          Heatbug.mo file     - ObjC object file
          Heatbug.o  file     - C++  object file

    Heatbug ObjC class definition and its implementation uses
    following Swarm ojects:
       - SwarmObject
       - UniformIntegerDist
       - UniformDoubleDist
       - Grid2d
       - Diffuse2d
       - Raster
       - Pixmap

    Heatbug class step method uses following Swarm ojects:
       - UniformIntegerDist
       - UniformDoubleDist
       - Grid2d
       - Diffuse2d


04  Heatbug's step method:

       - there are 11 calls to the ObjC/Swarm that must be
         translated to C++ calls. These methods are implemented in
         ObjC and must be called from C++.

         ie. ObjC export the functions to C++ (these ObjC exported
             interface functions are saved in the HeatbugExportObjC.m
             file - interface file 4.

         Object          Swarm Object        call  Methods
         --------------  ------------------  ----  ----------------------------------------
         uniformIntRand  UniformIntegerDist     3  [uniformIntRand getIntWithMin: withMax:]
         uniformDblRand  UniformDoubleDist      1  [uniformDblRand getIntWithMin: withMax:]
         heat            Diffuse2d              1  [heat getValueAtX: Y:]
                                                1  [heat findExtremeType: X: Y:]
                                                2  [heat addHeat: X: Y:]
         world           Grid2d                 1  [world getObjectAtX: Y:]
                                                2  [world putObject: atX: Y:]

         Note: - if we have fully translated (create an equivalent C++
                 class/interface for these Swarm objects), then there
                 is no need for "HeatbugExportObjC.m" interfacing file.

       - since we wants to implemented the "step" mmethod in C++,
         and will be able to call it from ObjC. Therefore, we need
         one C++ export function to ObjC (this C++ export interface
         function is stored in the HeatbugExportCpp - interface file 3.



04  Heatbugs environment modification

    10.01  $(SRMHOME)/etc/swarm/Makefile.rules
    10.01  $(SRMHOME)/etc/swarm/Makefile.common
    10.01  $(SRMHOME)/etc/swarm/Makefile.appl

10. Swarm v2.1.1 installation

    10.01  Packages

           swarm-2.1.1.tar.gz       1,466,703
           swarmapps-2.1.1.tar.gz     153,695

    10.02  Installation

           $ tar -zxvf swarm-2.1.1.tar.gz
           $ cd  swarm-2.1.1
           $ ./configure --with-defaultdir=/usr/local --prefix /home/richardl/swarm-2.1.1
           $ make
           $ make install


    10.03  Swarm Settings

           - assume Swarm is install in "/home/richardl/swarm-2.1.1"
             subdirectories.

           Makefile needs to include these:
              SWARMHOME = /home/richardl/swarm-2.1.1        ; Swarm install dir
              include $(SWARMHOME)/etc/swarm/Makefile.appl  ; include Swarm Makefile

    10.04  Swarm Libraries

           so ==> shared library

           /home/richardl/swarm-2.1.1/lib/

           libspace.so          libtkobjc.so       libcollections.so
           libanalysis.so       libtclobjc.so      libmisc.so
           libsimtoolsgui.so    libobjectbase.so   libobjc.so
           libsimtools.so       libactivity.so     libswarm.so
           librandom.so         libdefobj.so

           libobjc.so : is the ObjectC library from GNU GCC (??)

    10.05  Heatbugs examples

       (a) Actual compilation commands

       (b) Files

           Header file             ObjC file
           ----------------------  ---------------------------
                                   main.m
           Heatbug.h               Heatbug.m
           HeatSpace.h             HeatSpace.m
           HeatbugModelSwarm.h     HeatbugModelSwarm.m
           HeatbugObserverSwarm.h  HeatbugObserverSwarm.m
           HeatbugBatchSwarm.h     HeatbugMatchSwarm.m

           To compile to object file:

           main.o                : main.m + Heatbug.h
                                   + HeatSpace.h
                                   + HeatbugObserverSwarm.h
                                   + HeatbugBatchSwarm.h

           Heatbug.o             : Heatbug.m + Heatbug.h

           HeatSpace.o           : HeatSpace.m + HeatSpace.h

           HeatbugObserverSwarm.o: HeatbugObserverSwarm.m
                                   + HeatbugObserverSwarm.h
                                   + HeatbugModelSwarm.h

           HeatbugModelSwarm.o   : HeatbugModelSwarm.m + HeatbugModelSwarm.h

           HeatbugBatchSwarm.o   : HeatbugBatchSwarm.m + HeatbugBatchSwarm.h
                                   + HeatbugModelSwarm.h

       (c) Compilation

           ObjC-file:        ; Total 6 .o files created
              Heatbug.m
              HeatSpace.m
              main.m
              HeatbugModelSwarm.m
              HeatbugObserverSwarm.m
              HeatbugBatchSwarm.m

           ; Create 6 object-code files
           ;
           /usr/local/gcc2/bin/gcc  ; compile .m to .o
              -c                    ; output object file only (.o file)
              -g                    ; for use with symbolic debugger
              -O2                   ; optimise compilation
              -Wall                 ; all of the above '-W' options combined.
              -Wno-import           ; Inhibit warning about the use of '#import'
              -Wno-protocol         ; no warn if methods protocol not implemented
              -Werror               ; make all warnings into errors
              -D_GNU_SOURCE         ; Predefine name as a macro
              -DAPPNAME=heatbugs    ; Predefine name as a macro
              -DAPPVERSION=2.1.1    ; Predefine name as a macro
              -DBUGADDRESS=bug-swarm@swarm.org      ; Predefine name as a macro
              -I/home/richardl/swarm-2.1.1/include  ; additional header file dir
              objc-file.m                           ; source file

           ; Swarm linking batch file
           ;
           /bin/sh /home/richardl/swarm-2.1.1/bin/libtool-swarm
              --mode link /usr/local/gcc2/bin/gcc
              -g
              -O2
              -L/home/richardl/swarm-2.1.1/lib             ; swarm lib dir
              -rpath /home/richardl/swarm-2.1.1/lib
              -o heatbugs                                        ; exec file
              Heatbug.o HeatSpace.o main.o HeatbugModelSwarm.o   ; obj files
              HeatbugObserverSwarm.o HeatbugBatchSwarm.o
              -lswarm                       ; link to swarm lib
              -lobjc                        ; link to objc lib

           ; Actual linking command
           ;
           /usr/local/gcc2/bin/gcc          ; actual linking command
               -g                           ; for use with symbolic debugger
               -O2                          ; optimise compilation
               -o heatbugs                  ; output file name
               Heatbug.o HeatSpace.o main.o HeatbugModelSwarm.o   ; obj files
               HeatbugObserverSwarm.o HeatbugBatchSwarm.o
               -L/home/richardl/swarm-2.1.1/lib                   ; Swarm Lib
                 /home/richardl/swarm-2.1.1/lib/libswarm.so
               -L/home/richardl/swarm-2.1.1/src/space
               -L/home/richardl/swarm-2.1.1/src/analysis
               -L/home/richardl/swarm-2.1.1/src/simtoolsgui
               -L/home/richardl/swarm-2.1.1/src/simtools
               -L/home/richardl/swarm-2.1.1/src/random
               -L/home/richardl/swarm-2.1.1/src/tkobjc
               -L/home/richardl/swarm-2.1.1/src/tclobjc
               -L/home/richardl/swarm-2.1.1/src/objectbase
               -L/home/richardl/swarm-2.1.1/src/activity
               -L/home/richardl/swarm-2.1.1/src/defobj
               -L/home/richardl/swarm-2.1.1/src/collections
               -L/home/richardl/swarm-2.1.1/src/misc
               -L/home/richardl/swarm-2.1.1/libobjc
               -L/usr/lib -L/usr/X11R6/lib                        ; X11 Lib
               -L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/2.95/
                 /home/richardl/swarm-2.1.1/lib/libspace.so
                 /home/richardl/swarm-2.1.1/lib/libanalysis.so
                 /home/richardl/swarm-2.1.1/lib/libsimtoolsgui.so
                 /home/richardl/swarm-2.1.1/lib/libsimtools.so
                 /home/richardl/swarm-2.1.1/lib/librandom.so
                 /home/richardl/swarm-2.1.1/lib/libtkobjc.so
                 /home/richardl/swarm-2.1.1/lib/libtclobjc.so
                 /home/richardl/swarm-2.1.1/lib/libobjectbase.so
                 /home/richardl/swarm-2.1.1/lib/libactivity.so
                 /home/richardl/swarm-2.1.1/lib/libdefobj.so
                 /home/richardl/swarm-2.1.1/lib/libcollections.so
                 /home/richardl/swarm-2.1.1/lib/libmisc.so
                 /home/richardl/swarm-2.1.1/lib/libobjc.so
               -lgcc
               -lBLT24                           ; Plotting widgets TCL/Tk
               -ltk8.3                           ; ltk lib
               -ltcl8.3                          ; tcl lib
               -lXpm                             ; Xpm lib
               -lpng                             ; png lib
               -lz
               -lX11                             ; X11 lib
               -lm                               ; math lib
               -ldl /home/richardl/swarm-2.1.1/lib/libswarm.so
                    /home/richardl/swarm-2.1.1/lib/libobjc.so
               -L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/2.95/
               -lgcc /home/richardl/swarm-2.1.1/lib/libobjc.so
               -Wl,--rpath
               -Wl,/home/richardl/swarm-2.1.1/lib
               -Wl,--rpath -Wl,/usr/lib
               -Wl,--rpath -Wl,/usr/X11R6/lib

           ; Linking error msgs (GCC 3.1 only)
           ;

           /usr/bin/ld: warning: type and size of dynamic symbol
              `__objc_class_name_SwarmObject' are not defined
              `__objc_class_name_ZoomRaster' are not defined
              `__objc_class_name_CustomProbeMap' are not defined
              `__objc_class_name_GUISwarm' are not defined
              `__objc_class_name_Pixmap' are not defined
              `__objc_class_name_EZGraph' are not defined
              `__objc_class_name_Grid2d' are not defined
              `__objc_class_name_Object2dDisplay' are not defined
              `__objc_class_name_Value2dDisplay' are not defined
              `__objc_class_name_Swarm' are not defined
              `__objc_class_name_Diffuse2d' are not defined
              `__objc_class_name_Colormap' are not defined

           creating heatbugs


.SUFFIXES: .o .m .mo .mh .cd .cc


.h.cd:
	../classdesc -objc writeobjc < $< > $@

.h.mh:
	$(CPP) -g -c $(CLASSNAME).cc
	$(CPP) -g -DCNAME=$(CLASSNAME) -o $(TRANSLATOR) $(CPPOBJ) $(TRANSLATOR).cc
	$(TRANSLATOR)

.m.mo:
	$(CC) -c -o $@ -Wno-import $(CFLAGS) $<

.m.o:
	$(CC) -c -Wno-import $(CFLAGS) $<

CC = gcc
CPP = g++
CFLAGS = -g
LIBS = -L/usr/local/lib/gcc-lib/i686-pc-linux-gnu/2.95/ -L/usr/local/lib/libstdc++.so -lgcc -lobjc

CLASSNAME=myCounter
TRANSLATOR=write_objc
SRC=main.m
CPPOBJ=$(CLASSNAME).o
INTERFACE_OBJ= $(CLASSNAME)ExportCpp.o
OBJCOBJ=$(CLASSNAME).mo main.o
OBJ=$(CPPOBJ) $(INTERFACE_OBJ) $(OBJCOBJ)

#INTERFACE:


EXECUTABLE: $(CLASSNAME).cd $(CLASSNAME).mh $(OBJ)
	$(CPP) $(CFLAGS) -o main $(OBJ) $(LIBS)

.cc.o:
	/usr/local/gcc2/bin/g++ -g -c -DOBJC $(OPTFLAGS)  $<

main.o:                  main.m
$(CLASSNAME).cd:         $(CLASSNAME).h
$(CLASSNAME).mo:         $(CLASSNAME).mh $(CLASSNAME).m
$(CLASSNAME).o:          $(CLASSNAME).h $(CLASSNAME).cc
$(CLASSNAME).mh:         $(CLASSNAME).h $(CLASSNAME).cc
$(CLASSNAME)ExportCpp.o: $(CLASSNAME).h $(CLASSNAME)ExportCpp.cc


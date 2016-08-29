ifeq ($(SWARMHOME),)
SWARMHOME=/home/richardl/swarm-2.1.1
endif

APPLICATION=heatbugs
BUGADDRESS=bug-swarm@swarm.org
APPVERSION=2.1.1


CPPOBJ= Heatbug.o

OBJCOBJ=Heatbug.mo HeatSpace.o main.o HeatbugModelSwarm.o \
	HeatbugObserverSwarm.o HeatbugBatchSwarm.o HeatbugExportObjc.o

OBJECTS=$(OBJCOBJ) $(CPPOBJ)

DATAFILES = ant.png heatbugs.scm

include $(SWARMHOME)/etc/swarm/Makefile.appl



main.o: main.m Heatbug.mh HeatSpace.h HeatbugObserverSwarm.h HeatbugBatchSwarm.h

Heatbug.mo: Heatbug.m Heatbug.mh
Heatbug.o: Heatbug.cc Heatbug.h
HeatSpace.o: HeatSpace.m HeatSpace.h
HeatbugObserverSwarm.o: HeatbugObserverSwarm.m HeatbugObserverSwarm.h HeatbugModelSwarm.h
HeatbugModelSwarm.o: HeatbugModelSwarm.m HeatbugModelSwarm.h
HeatbugBatchSwarm.o: HeatbugBatchSwarm.m HeatbugBatchSwarm.h HeatbugModelSwarm.h
HeatbugExportObjc.o: HeatbugExportObjc.m






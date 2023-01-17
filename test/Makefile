VPATH= .. ../RavelCAPI

UNITTESTOBJS=main.o testTensorOps.o testXVector.o

CIVITAOBJS=$(filter-out ../tclmain.o,$(wildcard ../*.o))
FLAGS=-g -I.. -I../RavelCAPI
LIBS+=-L.. -lcivita -lboost_date_time -lboost_thread -lpthread -lUnitTest++ 
CPLUSPLUS=g++

ifdef GCOV
FLAGS+=-fprofile-arcs -ftest-coverage
endif

all: unittests 

unittests: $(UNITTESTOBJS) $(CIVITAOBJS) 
	$(CPLUSPLUS) $(FLAGS)  -o $@ $(UNITTESTOBJS) $(CIVITAOBJS) $(LIBS)

.cc.d: 
	$(CPLUSPLUS) $(FLAGS)  -MM -MG $< >$@

.cc.o: 
	$(CPLUSPLUS) -c $(FLAGS) $(OPT) -o $@ $<

BASIC_CLEAN=rm -rf *.o *~ "\#*\#" core *.d *.cd *.xcd *.gcda *.gcno

clean:
	-$(BASIC_CLEAN) unittests

.SUFFIXES: .d .h .cc .o

ifdef DEBUG
OPT=-g
else
OPT=-O3 -DNDEBUG
endif

CPLUSPLUS=g++

OBJS=hypercube.o index.o interpolateHypercube.o tensorOp.o xvector.o
VPATH=. RavelCAPI
FLAGS+=-I. -IRavelCAPI

libcivita.a: $(OBJS)
	ar r $@ $^

.cc.d: 
	$(CPLUSPLUS) $(FLAGS)  -MM -MG $< >$@

.cc.o: 
	$(CPLUSPLUS) -c $(FLAGS) $(OPT) -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif

tests: libcivita.a
	cd test; $(MAKE)

sure: tests
	test/unittests

BASIC_CLEAN=rm -rf *.o *~ "\#*\#" core *.d *.cd *.xcd *.gcda *.gcno

clean:
	-$(BASIC_CLEAN) libcivita.a
	cd RavelCAPI; $(MAKE) clean
	cd test; $(MAKE) clean

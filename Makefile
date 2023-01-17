.SUFFIXES: .d .h .cc .o .cd

ifdef DEBUG
OPT=-g
else
OPT=-O3 -DNDEBUG
endif

ifdef FPIC
OPT+=-fPIC
endif

CPLUSPLUS=g++

OBJS=hypercube.o index.o interpolateHypercube.o tensorOp.o xvector.o
FLAGS+=-I. -I$(HOME)/usr/include -I/usr/local/include 

ifeq ($(shell if which classdesc>&/dev/null; then echo 1; fi),1)
FLAGS+=-DCLASSDESC
endif

ifdef AEGIS
FLAGS+=-Werror -Wall -Wno-unused-variable -Wno-unused-function
endif

ifdef GCOV
FLAGS+=-fprofile-arcs -ftest-coverage
endif

libcivita.a: $(OBJS)
	ar r $@ $^

.cc.d: 
	$(CPLUSPLUS) $(FLAGS)  -MM -MG $< >$@

.cc.o: 
	$(CPLUSPLUS) -c $(FLAGS) $(OPT) -o $@ $<

.h.cd:
	classdesc -typeName -nodef -respect_private -i $< json_pack json_unpack >$@

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif

tests: libcivita.a
	cd test; $(MAKE)

sure: tests
	test/unittests

lcov:
	$(MAKE) clean
	-$(MAKE) GCOV=1 tests
	lcov -i -c -d . --no-external -o lcovi.info
	-$(MAKE) GCOV=1 sure
	lcov -c -d .  --no-external -o lcovt.info
	lcov -a lcovi.info -a lcovt.info -o lcov.info
	genhtml -o coverage lcov.info

BASIC_CLEAN=rm -rf *.o *~ "\#*\#" core *.d *.cd *.xcd *.gcda *.gcno

clean:
	-$(BASIC_CLEAN) libcivita.a
	cd test; $(MAKE) clean

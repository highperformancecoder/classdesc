.SUFFIXES: .d .h .cc .o

ifdef DEBUG
OPT=-g
else
OPT=-O3 -DNDEBUG
endif

CPLUSPLUS=g++

OBJS=hypercube.o index.o interpolateHypercube.o tensorOp.o xvector.o

libcivita.a: $(OBJS)
	ar r $@ $^

.cc.d: 
	$(CPLUSPLUS) $(FLAGS)  -MM -MG $< >$@

.cc.o: 
	$(CPLUSPLUS) -c $(FLAGS) $(OPT) -o $@ $<

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d)
endif

.SUFFIXES: .c .cc .o .d .h .cd
OBJS=RESTProcessExample.o SimpleBufferExample.o pyExample.o
CFLAGS=-g -I.. -I. -I../json5_parser/json5_parser -I/usr/local/include -fPIC -pthread
LIBS+=-L/usr/local/lib64 -lboost_system -lboost_thread -lpthread
VPATH=..
EXES=RESTProcess SimpleBuffer pyExample.so

CFLAGS+=$(shell pkg-config --cflags python3)

ifdef ASAN
CFLAGS+=-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
endif

all: $(EXES)

RESTProcess: RESTProcessExample.o
	g++  $^ $(CFLAGS) $(LIBS) -o $@

SimpleBuffer: SimpleBufferExample.o
	g++  $^ $(CFLAGS) $(LIBS) -o $@

pyExample.so: pyExample.o
	g++ -shared -o $@ $^ $(LIBS)

.h.cd:
	../classdesc -I .. -nodef -typeName -use_mbr_pointers -respect_private json_pack json_unpack <$< >$@
	../classdesc -I .. -nodef -use_mbr_pointers -onbase -overload -respect_private RESTProcess  <$< >>$@

.cc.o:
	g++ $(CFLAGS) -c -o $@ $<

.cc.d: 
	gcc $(CFLAGS) -w -MM -MG $< >$@

include $(OBJS:.o=.d)

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D 
	rm -rf $(EXES)

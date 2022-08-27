.SUFFIXES: .c .cc .o .d .h .cd
OBJS=RESTProcessExample.o
CFLAGS=-g -I.. -I. -I../json5_parser/json5_parser -I/usr/local/include -fPIC -std=c++17 -DUSE_UNROLLED -pthread
LIBS+=-L/usr/local/lib64 -lboost_thread -lpthread
VPATH=..

RESTProcess: $(OBJS)
	g++  $^ $(LIBS) -o $@

.h.cd:
	../classdesc -I .. -nodef -typeName -use_mbr_pointers -respect_private json_pack json_unpack <$< >$@
	../classdesc -I .. -nodef -use_mbr_pointers -onbase -overload -respect_private RESTProcess  <$< >>$@

.cc.o:
	g++ $(CFLAGS) -c -o $@ $<

.cc.d: 
	gcc $(CFLAGS) -w -MM -MG $< >$@

include $(OBJS:.o=.d) $(EXES:=.d)

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D 
	rm -rf RESTProcessExample

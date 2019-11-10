.SUFFIXES: .c .cc .o .d .h .cd
OBJS=RESTProcessExample.o
CFLAGS=-g -I.. -I. -fPIC -std=c++11
VPATH=..
LIBS=-ljson_spirit

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

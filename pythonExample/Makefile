.SUFFIXES: .c .cc .o .d .h .cd
OBJS=pythonExample.o
CFLAGS=-g `pkg-config --cflags python` -I.. -I. -fPIC
VPATH=..

example.so: $(OBJS)
	g++ -fPIC -shared -Wl,-soname,example $^ `pkg-config --libs python` -lboost_python -o $@

.h.cd:
	../classdesc -I .. -typeName -use_mbr_pointers pythonObject <$< >$@

.cc.o:
	g++ $(CFLAGS) -c -o $@ $<

.cc.d: 
	gcc $(CFLAGS) -w -MM -MG $< >$@

include $(OBJS:.o=.d) $(EXES:=.d)

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D 
	rm -rf $(EXES) 

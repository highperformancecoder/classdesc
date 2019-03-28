.SUFFIXES: .c .cc .o .d .h .cd
OBJS=pythonExample.o
CFLAGS=-g `pkg-config --cflags python3` -I.. -I. -fPIC
VPATH=..

example.so: $(OBJS)
	g++ -fPIC -shared -Wl,-soname,example $^ `pkg-config --libs python3` -lboost_python3 -o $@

.h.cd:
	../classdesc -I .. -typeName -use_mbr_pointers -onbase -overload python <$< >$@

.cc.o:
	g++ $(CFLAGS) -c -o $@ $<

.cc.d: 
	gcc $(CFLAGS) -w -MM -MG $< >$@

include $(OBJS:.o=.d) $(EXES:=.d)

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D 
	rm -rf $(EXES) 

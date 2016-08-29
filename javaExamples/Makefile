.SUFFIXES: .c .cc .o .d .h .cd .java .class

OBJS=../java/JNIEx.o
EXES=javaClassEx
DLLS=libJNIEx2.so
JAVACLASSES=$(patsubst %.java,%.class,$(wildcard *.java))

LIBS=-L.. 
CFLAGS=-fPIC -g -I.. -I. -I../java -DTR1 $(JDKINCLUDES) -DHAVE_LONGLONG
VPATH=..

ifdef GCOV
FLAGS+=-fprofile-arcs -ftest-coverage
endif

DIRS=$(HOME)/usr /usr/local /usr
LIBS+=$(foreach dir,$(DIRS),-L$(dir)/lib) -lm
CFLAGS+=$(foreach dir,$(DIRS),-I$(dir)/include)


OS=$(shell uname)
CYG_OS=$(shell uname|cut -f1 -d_)

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable -std=c++98
endif

CC=gcc
CPLUSPLUS=g++
LINK=$(CPLUSPLUS)


ifeq ($(OS),Linux)
LIBS+=-ldl
endif


all: $(EXES) $(DLLS) $(JAVACLASSES)

$(EXES): %: %.o  
	$(CPLUSPLUS) $(CFLAGS) -o $@ $< $(LIBS)

$(DLLS): lib%.so: %.o $(OBJS)
	$(CPLUSPLUS) $(CFLAGS) -shared -Wl,--export-dynamic,-soname,$@ -o $@.tmp $< $(OBJS)
	-mkdir -p $*
	(cd $*; ../../java/compileClass ../$@.tmp)
	$(CPLUSPLUS) $(CFLAGS) -shared -Wl,--export-dynamic,-soname,$@ -o $@ $(DLLSRC:%.cc=%.o) $< $(OBJS) $*/*.cc
	(cd $*; jar cvf ../$*.jar *.class)
	rm $@.tmp

HAVE_GCC=$(words $(foreach dir,$(subst :, ,$(PATH)),$(wildcard $(dir)/gcc $(dir)/gcc.exe)))
ifneq ($(HAVE_GCC),0)
.c.d: 
	gcc $(CFLAGS) -w -MM $< >$@

.cc.d: 
	gcc $(CFLAGS) -w -MM -MG $< >$@
else
.c.d: 
	if [ -f $@ ]; then touch $@; else echo "run make depend on a computer with gcc installed"; exit 1; fi

.cc.d: 
	if [ -f $@ ]; then touch $@; else echo "*** run make depend on a computer with gcc installed ***"; exit 1; fi
endif

depend: $(OBJS:.o=.d) $(EXES:=.d) $(DLLS:lib%.so=%.d)
	cat *.d >depend

include depend

.cc.o:
	$(CPLUSPLUS) $(CFLAGS) -c -o $@ $<

.c.o: 
	$(CC) $(CFLAGS) -c $< 

.h.cd:
	../classdesc -I .. -nodef pack unpack <$< >$@
	../classdesc -I .. -nodef -respect_private dump javaClass <$< >>$@

$(JAVACLASSES): $(DLLS)

.java.class:
	-rm $@
	javac -cp .:../java:JNIEx2.jar $<

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe *.class
	rm -rf $(EXES) $(DLLS) $(DLLS:lib%.so=%.jar) $(DLLS:lib%.so=%) include-paths cxx_repository
	rm -rf *.a


.SUFFIXES: .c .cc .o .d .h .cd .java .class

OBJS=JNIEx.o
EXES=dumpClass
JAVACLASSES=$(patsubst %.java,%.class,$(wildcard *.java))

LIBS=-L.. 
CFLAGS=-fPIC -g -I.. -I. -DTR1 $(JDKINCLUDES) -DHAVE_LONGLONG
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


all: $(EXES) compileClass $(JAVACLASSES)

$(EXES): %: %.o  
	$(CPLUSPLUS) $(CFLAGS) -o $@ $< $(LIBS)

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

compileClass: compileClass.o
	$(CPLUSPLUS) $(CFLAGS) -Wl,--export-dynamic -o $@ $^ -ldl

$(JAVACLASSES): $(DLLS)

.java.class:
	-rm $@
	javac $<

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe *.class
	rm -rf $(EXES) include-paths cxx_repository
	rm -rf *.a


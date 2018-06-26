.SUFFIXES: .c .cc .o .d .h .cd

#define NOGUI if X-windows not installed
NOGUI=

OBJS+=polymorph.o # needed to ensure polymorph.d is created and included
EXES=heat graphnode ptrflag refnode old-polymorph polymorph2 internal_type
# note the internal type example cannot be correctly linked with g++ or icc!
LIBS=-L.. -L/usr/X11R6/lib
CFLAGS=-g -I.. -I. -I/usr/X11R6/include -DRECUR_MAX=50 -DTR1

ifdef GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

DIRS=$(HOME)/usr /usr/local /usr
LIBS+=$(foreach dir,$(DIRS),-L$(dir)/lib) -lm
CFLAGS+=$(foreach dir,$(DIRS),-I$(dir)/include)

VPATH=..

OS=$(shell uname)
CYG_OS=$(shell uname|cut -f1 -d_)

ifndef NOGUI
TCL_LIB=$(shell grep TCL_LIB_SPEC $(word 1,$(wildcard $(foreach dir,$(DIRS),$(dir)/lib*/tclConfig.sh))) |cut -f2 -d\')
TK_LIB=$(shell grep TK_LIB_SPEC $(word 1,$(wildcard $(foreach dir,$(DIRS),$(dir)/lib*/tkConfig.sh)))|cut -f2 -d\')
LIBS+=$(TK_LIB) $(TCL_LIB) -lm
RASTER=raster.o
ifneq ($(CYG_OS),CYGWIN)
LIBS+=-L/usr/X11/lib -lX11
endif
else
CFLAGS+=-DAEGIS_TEST
endif

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable -std=c++98
endif

CC=gcc
CPLUSPLUS=g++
LINK=$(CPLUSPLUS)

ifeq ($(CYG_OS),CYGWIN)
LINK+= -Wl,--enable-auto-import
endif

ifndef GCC 
ifeq ($(OS),OSF1)
CC=cc 
CPLUSPLUS=cxx 
CFLAGS+=-D__USE_STD_IOSTREAM 
endif 
ifeq ($(OS),IRIX64) 
CC=cc 
CPLUSPLUS=CC 
CFLAGS+=-LANG:std 
endif 
endif 

ifeq ($(OS),Linux)
LIBS+=-ldl
endif


all: $(EXES) polymorph

$(EXES): %: %.o  $(RASTER)
	$(LINK) $(CFLAGS) -o $@ $< $(RASTER) $(LIBS)

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

depend: $(OBJS:.o=.d) $(EXES:=.d)
	cat *.d >depend

include depend

.cc.o:
	$(CPLUSPLUS) $(CFLAGS) -c -o $@ $<

.c.o: 
	$(CC) $(CFLAGS) -c $< 

.h.cd:
	../classdesc -I .. -typeName -use_mbr_pointers pack unpack  xml_pack xml_unpack <$< >$@

#polymorph2.cd: polymorph2.h
#	../classdesc -I .. -typeName pack unpack xml_pack xml_unpack <$< >$@

polymorph.cd: polymorph.h
	rm -rf $*.cdir 
	mkdir -p $*.cdir 
	../classdesc -I .. -typeName -workdir $*.cdir -include $<  pack unpack xml_pack xml_unpack <$< >$@

polymorph.a: polymorph.cd
	$(MAKE) $(patsubst %.cc,%.o,$(wildcard $*.cdir/*.cc))
	ar r polymorph.a $*.cdir/*.o
ifeq ($(OS),Darwin)
	ranlib polymorph.a
endif

polymorph: polymorph.o polymorph.a
	$(LINK) $(CFLAGS) -o $@ $^ $(LIBS)

test:
	echo $(TCL_LIB)

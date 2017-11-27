.SUFFIXES: .c .cc .o .d .h .cd

#define NOGUI if X-windows not installed
NOGUI=

EXES=c++Features type_traits_check moveOperators
OBJS=

# note the internal type example cannot be correctly linked with g++ or icc!
LIBS=-L.. -L/usr/X11R6/lib
CFLAGS=-g -DXDR_PACK -std=c++11 -I../.. -I. -I/usr/X11R6/include 

ifdef GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable
endif



DIRS=$(HOME)/usr /usr/local /usr
LIBS+=$(foreach dir,$(DIRS),-L$(dir)/lib) ../../xdr_pack.o -lm 
CFLAGS+=$(foreach dir,$(DIRS),-I$(dir)/include)

JSON_HEADER=$(firstword $(foreach dir,$(DIRS),$(wildcard $(dir)/include/json_spirit.h)))

OS=$(shell uname)
CYG_OS=$(shell uname|cut -f1 -d_)

CC=gcc
CPLUSPLUS=g++
LINK=$(CPLUSPLUS)

ifeq ($(CYG_OS),CYGWIN)
LINK+= -Wl,--enable-auto-import
endif

VPATH=..

ifndef GCC 
ifeq ($(OS),OSF1)
CC=cc 
CPLUSPLUS=cxx 
#CFLAGS+=-DEXPLICIT_FRIEND_TEMPLATE_ARGS_REQUIRED=0
CFLAGS+=-D__USE_STD_IOSTREAM 
endif 
ifeq ($(OS),IRIX64) 
CC=cc 
CPLUSPLUS=CC 
CFLAGS+=-DEXPLICIT_FRIEND_TEMPLATE_ARGS_REQUIRED=0 -LANG:std 
endif 
endif 

ifeq ($(OS),Linux)
LIBS+=-ldl
endif


all: $(EXES)

$(EXES): %: %.o  $(OBJS)
	$(LINK) $(CFLAGS) -o $@  $< $(OBJS) $(LIBS)

$(MPIEXES): %: %.o  $(OBJS)
	mpicxx $(CFLAGS) -o $@  $< $(OBJS) $(LIBS)

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
	../../classdesc -I .. -nodef -typeName -i $< pack unpack >$@
# check that duplicate typeNames can be issued
#	../classdesc -I .. -nodef -typeName <$< >>$@
	../../classdesc -I .. -nodef -typeName -respect_private -i $< xml_pack xml_unpack dump xsd_generate  >>$@

xml_unpack_base.cd: xml_unpack_base.h
	../../classdesc -I .. -nodef pack unpack <$< >$@

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe
	rm -rf $(EXES) include-paths cxx_repository
	rm -rf *.a *.xmltmp

test1:
	echo $(JSON_HEADER)

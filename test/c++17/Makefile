.SUFFIXES: .c .cc .o .d .h .cd

#define NOGUI if X-windows not installed
NOGUI=

EXES=testFunctional c++Features testXSDGenerate
OBJS=

# note the internal type example cannot be correctly linked with g++ or icc!
LIBS=-L.. -L/usr/X11R6/lib
CFLAGS=-g -std=c++17 -DUSE_UNROLLED -I../.. -I. -I/usr/X11R6/include -I../../json5_parser/json5_parser

ifdef GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable -Wno-unused-function -Wno-unused-local-typedefs
endif

ifdef ASAN
CFLAGS+=-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
endif

DIRS=$(HOME)/usr /usr/local /usr
LIBS+=$(foreach dir,$(DIRS),-L$(dir)/lib) -lm 
CFLAGS+=$(foreach dir,$(DIRS),-I$(dir)/include)

LIBS+=`pkg-config --libs libtirpc`

JSON_HEADER=$(firstword $(foreach dir,$(DIRS),$(wildcard $(dir)/include/json_spirit.h)))

OS=$(shell uname)
CYG_OS=$(shell uname|cut -f1 -d_)

CC=gcc
CPLUSPLUS=g++
LINK=$(CPLUSPLUS)

ifeq ($(CYG_OS),CYGWIN)
LINK+= -Wl,--enable-auto-import
endif

VPATH=../..

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


all: sure

$(EXES): %: %.o  $(OBJS)
	$(LINK) $(CFLAGS) -o $@  $< $(OBJS) $(LIBS)

sure: $(EXES)
	sh runtests.sh $(EXES) 

$(MPIEXES): %: %.o  $(OBJS)
	mpicxx $(CFLAGS) -o $@  $< $(OBJS) $(LIBS)

# needed because many examples do not explicitly reference xml_pack/unpack
$(EXES:%=%.o): xml_common.cd

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

ifneq ($(MAKECMDGOALS),clean)
include $(OBJS:.o=.d) $(EXES:=.d)
endif

.cc.o:
	$(CPLUSPLUS) $(CFLAGS) -c -o $@ $<

.c.o: 
	$(CC) $(CFLAGS) -c $< 

.h.cd:
	../../classdesc -I .. -nodef -typeName -i $< pack unpack >$@
# check that duplicate typeNames can be issued
#	../classdesc -I .. -nodef -typeName <$< >>$@
	../../classdesc -I .. -nodef -typeName -respect_private -i $< json_pack json_unpack xml_pack xml_unpack dump xsd_generate  >>$@

xml_unpack_base.cd: xml_unpack_base.h
	../../classdesc -I .. -nodef pack unpack <$< >$@

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe
	rm -rf $(EXES) include-paths cxx_repository
	rm -rf *.a *.xmltmp

test1:
	echo $(JSON_HEADER)

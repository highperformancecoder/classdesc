.SUFFIXES: .c .cc .o .d .h .cd

#define NOGUI if X-windows not installed
NOGUI=

EXES=mandelbrot heat

DIRS=$(ECOLAB_HOME) $(HOME)/usr /usr/local /opt/local /usr/X11R6
search=$(firstword $(foreach dir,$(DIRS) /usr,$(wildcard $(dir)/$(1))))

CFLAGS=-g -I. -I.. -I$(HOME)/usr/include -I/usr/local/include -DTR1
LIBS=-L.. -L$(HOME)/usr/lib
VPATH=..

ifdef GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

ifdef ASAN
CFLAGS+=-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
endif

ifndef NOGUI
LIBS+=$(shell . $(call search,lib*/tkConfig.sh); echo $$TK_LIB_FLAG $$TK_STUB)
LIBS+=$(shell . $(call search,lib*/tclConfig.sh); echo $$TCL_LIB_SPEC $$TCL_STUB_LIB_FLAG)
LIBS+=$(shell . $(call search,lib*/tkConfig.sh); echo $$TK_LIBS)
LIBS+=$(TCLLIBS) -lm
RASTER=raster.o
else
CFLAGS+=-DAEGIS_TEST
endif

CC=mpicc
HAVE_mpiCC=$(shell if which mpiCC>&/dev/null; then echo 1; fi)
ifeq ($(HAVE_mpiCC),1)
CPLUSPLUS=mpiCC
else
# newer versions of mpich use this name!!
CPLUSPLUS=mpicxx
endif
LINK=$(CPLUSPLUS)

ifeq ($(CC),icc)
CPLUSPLUS=icc
LINK=icc
endif

OS=$(shell uname)
HAVE_GCC=$(words $(foreach dir,$(subst :, ,$(PATH)),$(wildcard $(dir)/gcc $(dir)/gcc.exe)))

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

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable -Wno-unused-function 
CPLUSPLUS+=-std=c++98
endif

all: $(EXES)

$(EXES): %: %.o  $(RASTER)
	$(CPLUSPLUS) $(CFLAGS) -o $@ $< $(RASTER) $(LIBS)

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

ifneq ($(MAKECMDGOALS),clean)
include depend
endif

.cc.o:
	$(CPLUSPLUS) $(CFLAGS) -c $<

.c.o: 
	$(CC) $(CFLAGS) -c $<

.h.cd:
	../classdesc -I .. pack unpack <$< >$@

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe
	rm -rf $(EXES) include-paths cxx_repository

test:
	echo $(HAVE_GCC)

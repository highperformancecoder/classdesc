.SUFFIXES: .c .cc .o .d .h .cd

#define NOGUI if X-windows not installed
NOGUI=

EXES=testXML_base testXMLcd testXSDgenerate testTypeName testDump	\
testfunctional testRef testJavaClass test_stl anonNamespace		\
nestedClass testConst testBinStream exclude testJsonSharedPtr		\
testExclude ticket20-example enumKeys miscMemberTypes templateArgs	\
test_const_method privateBase random_init stringInheritance		\
cpdoexample privateEnum testFnPtr testContainersClear ticket43-example  \
hyphen_in_template_parameter testXMLCDATA explicitTemplateArg           \
complex_tcl_args referenceMember testBoundMethod testPackFunctor testJsoncd testJsonStream 

MPIEXES=mpibuf_array 
OBJS=testXMLcd_other.o

# note the internal type example cannot be correctly linked with g++ or icc!
LIBS=-L.. -L/usr/X11R6/lib `pkg-config --libs libtirpc` -lboost_thread
CFLAGS=-g -I.. -I. -I../json5_parser/json5_parser -I/usr/X11R6/include -DRECUR_MAX=50 -DTR1 -DHAVE_LONGLONG

ifdef GCOV
CFLAGS+=-fprofile-arcs -ftest-coverage
endif

ifdef AEGIS
CFLAGS+=-Werror -Wall -Wno-unused-variable -Wno-unused-local-typedefs -std=c++98
endif


DIRS=$(HOME)/usr /usr/local /usr
LIBS+=$(foreach dir,$(DIRS),-L$(dir)/lib) ../xdr_pack.o -lm 
CFLAGS+=$(foreach dir,$(DIRS),-I$(dir)/include)

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

ifneq ($(MAKECMDGOALS),clean)
include depend
endif

.cc.o:
	$(CPLUSPLUS) $(CFLAGS) -c -o $@ $<

.c.o: 
	$(CC) $(CFLAGS) -c $< 

.h.cd:
	../classdesc -I .. -nodef -typeName -use_mbr_pointers -i $< random_init pack unpack >$@
# check that duplicate typeNames can be issued
	../classdesc -I .. -nodef -typeName -respect_private -use_mbr_pointers -onbase -i $< xml_pack xml_unpack dump xsd_generate  >>$@
	../classdesc -I .. -nodef -respect_private -typeName -onbase -use_mbr_pointers  -i $< json_pack json_unpack >>$@

xml_unpack_base.cd: xml_unpack_base.h
	../classdesc -I .. -nodef pack unpack <$< >$@

clean: 
	rm -f *.o  *~ "\#*\#" core *.exh *.exc *.d *.cd *,D depend *.exe
	rm -rf $(EXES) include-paths cxx_repository
	rm -rf *.a *.xmltmp

test1:
	echo $(JSON_HEADER)

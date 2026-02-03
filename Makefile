# set this variable to force use of gcc
CC=gcc
CPLUSPLUS=g++
# -I. should _not_ be necessary, but seems to be necessary on OpenSUSE
# -I/usr/include/tirpc is needed for Fedora
FLAGS=-g -I. -I/usr/include/tirpc

CDHEADERS=multiArray object poly polyBase polyRESTProcess polyRESTProcessBase polyXMLBase RESTProcess_base signature stringKeyMap xml_pack_base xml_unpack_base xml_common
DESCRIPTORS=dump pack json_pack random_init RESTProcess xml_pack xsd_generate typeName
CDFILES=$(foreach d,$(DESCRIPTORS),$(foreach h,$(CDHEADERS),$(h)-$(d).cd))

ifdef GCOV
GCOV_FLAGS+=-fprofile-arcs -ftest-coverage
FLAGS+=-fprofile-arcs -ftest-coverage
endif

ifdef AEGIS
TRAVIS=1
endif

ifdef ASAN
FLAGS+=-fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls -O1
endif

ifdef TRAVIS
FLAGS+=-Werror -Wall -Wno-unused-variable -Wno-unused-function -std=c++98
endif

XDR=1

PREFIX=${HOME}/usr

OS=$(shell uname)

ifdef AEGIS  #keep a cached copy of sysincludes in home directory for AEGIS
SYSINCLUDES=${HOME}/usr/ecolab/sysincludes
else
SYSINCLUDES=sysincludes
endif

# Utility programs
EXES=classdesc insert-friend
# wildcard runs before functiondb.h is created on some systems
INCLUDES=$(wildcard *.h) functiondb.h unpack_base.h json_unpack_base.h $(wildcard json5_parser/json5_parser/*.h) 

# canonicalise CYGWIN's OS name
ifeq ($(findstring CYGWIN,$(OS)),CYGWIN)
OS=CYGWIN
XDR=
# CYGWIN has a buggy fsetpos routine
FLAGS+=-DUSE_FSEEK
CPLUSPLUS=g++ -Wl,--enable-auto-import
endif

ifeq ($(findstring MINGW,$(OS)),MINGW)
OS=MINGW
XDR=
endif

ifeq ($(findstring Darwin,$(OS)),Darwin)
# Apple has removed TR1, but not set the C++ standard :P
FLAGS+=-std=c++11
endif

FLAGS+=-I/usr/X11R6/include

ifdef XDR
FLAGS+=-DXDR_PACK -DHAVE_LONGLONG
endif

# Some compilers allow friend functions to refer to type without
# explicit template  arguments.
ifndef GCC
ifeq ($(OS),OSF1)
CC=cc
CPLUSPLUS=cxx  -D__USE_STD_IOSTREAM
endif
ifeq ($(OS),IRIX64)
CC=cc
CPLUSPLUS=CC -LANG:std
endif
endif

.SUFFIXES: .cc .c .o .h .d 

.cc.o: 
	$(CPLUSPLUS) -c $(FLAGS)  $<

.c.o: 
	$(CC) -c $(FLAGS) $<

ifdef AEGIS
all: aegis-all
else
all: build
endif

ifdef XDR
XDRPACK=xdr_pack.o
else
XDRPACK=
endif

build: $(EXES) $(DESCRIPTORS:%=%-allCDs.h) $(CDFILES) $(XDRPACK) functiondb.h unpack_base.h json_unpack_base.h 

unpack_base.h:
	-ln -s pack_base.h unpack_base.h

json_unpack_base.h:
	-ln -s json_pack_base.h json_unpack_base.h

aegis-all: build latex-docs
	echo $(LD_LIBRARY_PATH)
	-ln -s pack_base.h unpack_base.h
	if which mpicxx; then	cd mpi-examples && $(MAKE) CPLUSPLUS=mpicxx CC=mpicc; fi
	cd examples && $(MAKE) NOGUI=1
	cd pythonExample && $(MAKE) 
#NB if these builds fail, check for jni.h and jni_md.h in the standard include path
# This is now broken, as of OpenJDK 15. Not worth supporting. 
#	if which javac; then cd java && $(MAKE) && cd ../javaExamples && $(MAKE); fi
	cd test && $(MAKE)
	cd test/c++11 && $(MAKE) sure
	cd test/c++17 && $(MAKE) sure
	cd RESTProcessExample && $(MAKE)
#	-cd objc-examples && $(MAKE)

$(EXES): %: %.cc tokeninput.h
	$(CPLUSPLUS) $(FLAGS) $< -o $@

xdr_pack.o: xdr_pack.cc
	$(CPLUSPLUS) $(FLAGS) -DTR1 -c $<

functiondb.h: functiondb.sh
	-rm $@
	bash $< >$@

clean: 
	rm -f *.o  *~ "\#*\#" core *.cd *.exh *.exc *.d *,D *.exe
	rm -rf $(EXES) include-paths cxx_repository
	cd mpi-examples && $(MAKE) clean
	cd examples && $(MAKE) clean
	cd objc-examples && $(MAKE) clean
	cd java && $(MAKE) clean
	cd javaExamples && $(MAKE) clean
	cd pythonExample && $(MAKE) clean
	cd RESTProcessExample && $(MAKE) clean
	cd test && $(MAKE) clean
	cd test/c++11 && $(MAKE) clean
	cd doc && rm -f *~ *.aux *.dvi *.log *.blg *.toc *.lof
	rm -rf ii_files */ii_files

# test compile Latex docs, if latex is on system
latex-docs:
	if which latex; then cd doc; rm -f *.out *.aux *.dvi *.log *.blg *.toc *.lof; latex -interaction=batchmode classdesc; fi

install: build
	-mkdir $(PREFIX)
	-mkdir $(PREFIX)/bin
	-mkdir $(PREFIX)/include
ifeq ($(OS),CYGWIN)
	cp -f $(EXES:%=%.exe)  $(PREFIX)/bin
else
	cp -f $(EXES)  $(PREFIX)/bin
endif
	cp -f fix-privates $(PREFIX)/bin
	cp -f $(INCLUDES) *.cd $(PREFIX)/include
#	-ln -s $(PREFIX)/include/pack_base.h $(PREFIX)/include/unpack_base.h
#	cp -r $(SYSINCLUDES) $(PREFIX)

sure: aegis-all
	-ln -s pack_base.h unpack_base.h
	-cd mpi-examples && $(MAKE) clean && $(MAKE) NOGUI=1
	-cd examples && $(MAKE) clean && $(MAKE) NOGUI=1
#	-cd objc-examples && $(MAKE)
	sh runtests "$(CPLUSPLUS) $(GCOV_FLAGS)" test/00/*.sh

c++11-sure: clean 
	$(MAKE) CPLUSPLUS="g++ --std=c++11" classdesc
# cannot override CPLUSPLUS with mpi-examples 
	if which mpicxx; then	cd mpi-examples && $(MAKE); fi
	$(MAKE) CPLUSPLUS="g++ --std=c++11" sure

# don't bother with MPI or Java tests on Travis
travis-test: build 
	cd examples && $(MAKE) NOGUI=1
	cd test && $(MAKE)
	cd test/c++11 && $(MAKE)
	cd RESTProcessExample && $(MAKE)
	sh runtests "g++ $(GCOV_FLAGS)" `ls test/00/*.sh|grep -v t0002a|grep -v t0003a|grep -v t0041a|grep -v t0051a`

dist:
	sh makeDist.sh

# cd file generation rules
$(DESCRIPTORS:%=%-allCDs.h): Makefile createCDs.sh
	./createCDs.sh $(subst -allCDs.h,,$@) $(CDHEADERS)

%-dump.cd: %.h classdesc
	./classdesc  -nodef -onbase -i $< dump >$@

%-pack.cd: %.h classdesc
	./classdesc  -nodef -onbase -i $< pack unpack >$@

%-json_pack.cd: %.h classdesc
	./classdesc  -nodef -onbase -respect_private -i $< json_pack json_unpack >$@

%-xml_pack.cd: %.h classdesc 
	./classdesc  -nodef -onbase -respect_private -i $< xml_pack xml_unpack >$@

%-xsd_generate.cd: %.h classdesc 
	./classdesc  -nodef -onbase -respect_private -i $< xsd_generate >$@

%-random_init.cd: %.h classdesc
	./classdesc  -nodef -onbase -i $< random_init >$@

%-RESTProcess.cd: %.h classdesc
	./classdesc  -nodef -onbase -respect_private -i $< RESTProcess >$@

%-typeName.cd: %.h classdesc
	./classdesc  -nodef -typeName -i $<  >$@

# install documentation on SourceForge
DOCPREFIX=web.sf.net:/home/project-web/classdesc/htdocs/doc

install-doc:
	doxygen
	-cd doc; sh Makedoc
	rsync -e ssh -r -z --progress --delete doc/classdesc $(DOCPREFIX)
	rsync -e ssh -r -z --progress --delete html/ $(DOCPREFIX)/doxygen/

lcov:
	$(MAKE) clean
	$(MAKE) GCOV=1 sure
	lcov -i -c -d . --no-external -o lcov.info
	lcov -c -d . --no-external -o lcov.info
	genhtml -o coverage lcov.info

# Makefile for redislib directory
#****************************************************************
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OBJECTS = \
   	genlib.o \
    simpio.o \
    strlib.o \
    queue.o \
    queuelist.o \
    random.o \
    cmpfn.o \
    ref.o \
    scanadt.o \
    set.o \
    foreach.o \
    stack.o \
    stacklist.o \
    symtab.o \
    iterator1.o \
    iterator2.o \
    exception.o \
    graphics.o \
    xmanager.o \
    xdisplay.o \
    xcompat.o \
    glibrary.o \
    bst.o

CSLIB = redislib.a

# Default settings
CC = gcc
STD=-std=c99 -pedantic -DREDIS_STATIC=''
WARN=-Wall -W
OPT=$(OPTIMIZATION)

PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin
INSTALL=install

# Default allocator
ifeq ($(uname_S),Linux)
	MALLOC=jemalloc
else
	MALLOC=libc
endif

# Backwards compatibility for selecting an allocator
ifeq ($(USE_TCMALLOC),yes)
	MALLOC=tcmalloc
endif

ifeq ($(USE_TCMALLOC_MINIMAL),yes)
	MALLOC=tcmalloc_minimal
endif

ifeq ($(USE_JEMALLOC),yes)
	MALLOC=jemalloc
endif

ifeq ($(USE_JEMALLOC),no)
	MALLOC=libc
endif

FINAL_CFLAGS=$(STD) $(WARN) $(OPT) $(DEBUG) $(CFLAGS) $(REDIS_CFLAGS)
FINAL_LDFLAGS=$(LDFLAGS) $(REDIS_LDFLAGS) $(DEBUG)
FINAL_LIBS=-lm
DEBUG=-g -ggdb

ifeq ($(MALLOC),tcmalloc)
	FINAL_CFLAGS+= -DUSE_TCMALLOC
	FINAL_LIBS+= -ltcmalloc
endif

ifeq ($(MALLOC),tcmalloc_minimal)
	FINAL_CFLAGS+= -DUSE_TCMALLOC
	FINAL_LIBS+= -ltcmalloc_minimal
endif

ifeq ($(MALLOC),jemalloc)
	DEPENDENCY_TARGETS+= jemalloc
	FINAL_CFLAGS+= -DUSE_JEMALLOC -I/usr/local/jemalloc/include -L/usr/local/jemalloc/lib -Wl,-rpath,/usr/local/jemalloc/lib
	FINAL_LIBS+= -ljemalloc
endif

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(CSLIB) gccj

# ***************************************************************
# C compilations
genlib.o: genlib.c genlib.h exception.h
	$(CC) $(FINAL_CFLAGS) -c genlib.c $(FINAL_LIBS)
bst.o: bst.c bst.h cmpfn.h genlib.h
	$(CC) $(CFLAGS) -c bst.c

cmpfn.o: cmpfn.c cmpfn.h strlib.h genlib.h
	$(CC) $(CFLAGS) -c cmpfn.c

iterator1.o: iterator1.c iterator1.h genlib.h strlib.h symtab.h
	$(CC) $(CFLAGS) -c iterator1.c

iterator2.o: iterator2.c iterator2.h cmpfn.h genlib.h itertype.h
	$(CC) $(CFLAGS) -c iterator2.c

queue.o: queue.c queue.h genlib.h
	$(CC) $(CFLAGS) -c queue.c

queuelist.o: queuelist.c queuelist.h genlib.h
	$(CC) $(CFLAGS) -c queuelist.c

random.o: random.c random.h genlib.h
	$(CC) $(CFLAGS) -c random.c

ref.o: ref.c ref.h genlib.h
	$(CC) $(CFLAGS) -c ref.c

scanadt.o: scanadt.c scanadt.h genlib.h strlib.h
	$(CC) $(CFLAGS) -c scanadt.c

set.o: set.c set.h bst.h genlib.h iterator2.h itertype.h
	$(CC) $(CFLAGS) -c set.c

foreach.o: foreach.c foreach.h genlib.h iterator2.h
	$(CC) $(CFLAGS) -c foreach.c

simpio.o: simpio.c simpio.h strlib.h genlib.h
	$(CC) $(CFLAGS) -c simpio.c

stack.o: stack.c stack.h genlib.h
	$(CC) $(CFLAGS) -c stack.c

stacklist.o: stacklist.c stacklist.h genlib.h
	$(CC) $(CFLAGS) -c stacklist.c

strlib.o: strlib.c strlib.h genlib.h
	$(CC) $(CFLAGS) -c strlib.c

symtab.o: symtab.c symtab.h strlib.h genlib.h itertype.h cmpfn.h
	$(CC) $(CFLAGS) -c symtab.c

exception.o: exception.c exception.h genlib.h
	$(CC) $(CFLAGS) -c exception.c

graphics.o: graphics.c graphics.h extgraph.h xmanager.h xcompat.h \
            glibrary.h genlib.h gcalloc.h simpio.h strlib.h Makefile
	$(CC) $(CONFIG) $(CFLAGS) -c graphics.c

xmanager.o: xmanager.c xmanager.h xdisplay.h xcompat.h glibrary.h \
	    genlib.h exception.h simpio.h Makefile
	$(CC) $(CONFIG) $(CFLAGS) -c xmanager.c

xdisplay.o: xdisplay.c xdisplay.h xmanager.h glibrary.h genlib.h strlib.h \
            Makefile
	$(CC) $(CONFIG) $(CFLAGS) -c xdisplay.c

xcompat.o: xcompat.c xcompat.h Makefile
	$(CC) $(CONFIG) $(CFLAGS) -c xcompat.c

glibrary.o: glibrary.c glibrary.h genlib.h
	$(CC) $(CFLAGS) -c glibrary.c


$(CSLIB): $(OBJECTS)
	-rm -f $(CSLIB)
	ar cr $(CSLIB) $(OBJECTS)
	ranlib $(CSLIB)

# ***************************************************************
# Entry to reconstruct the gccx script

gccj: Makefile
	@echo '#! /bin/csh -f' > gccj
	@echo 'set INCLUDE =' `pwd` >> gccj
	@echo 'set CSLIB = $$INCLUDE/redislib.a' >> gccj
	@echo 'set LIBRARIES = ($$CSLIB -L/usr/local/jemalloc/lib -lX11 -lm -Wl,-rpath,/usr/local/jemalloc/lib -ljemalloc)' >> gccj
	@echo 'foreach x ($$*)' >> gccj
	@echo '  if ("x$$x" == "x-c") then' >> gccj
	@echo '    set LIBRARIES = ""' >> gccj
	@echo '    break' >> gccj
	@echo '  endif' >> gccj
	@echo 'end' >> gccj
	@echo 'gcc -g -I$$INCLUDE -I/usr/local/jemalloc/include/jemalloc $$* $$LIBRARIES' >> gccj
	@chmod a+x gccj
	@echo '[gccj script created]'


# Makefile for redislib directory
#****************************************************************
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OBJECTS = \
   	genlib.o \
   	strlib.o \
   	simpio.o 

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
	FINAL_CFLAGS+= -DUSE_JEMALLOC -I./jemalloc/include
	FINAL_LIBS+= ./jemalloc/lib/libjemalloc.a
endif

# ***************************************************************
# Entry to bring the package up to date
#    The "make all" entry should be the first real entry

all: $(CSLIB) gcct

# ***************************************************************
# C compilations
genlib.o: genlib.c genlib.h exception.h
	$(CC) $(FINAL_CFLAGS) -c genlib.c $(FINAL_LIBS)
strlib.o: strlib.c strlib.h genlib.h
	$(CC) $(CFLAGS) -c strlib.c
simpio.o: simpio.c simpio.h strlib.h genlib.h
	$(CC) $(CFLAGS) -c simpio.c


$(CSLIB): $(OBJECTS)
	-rm -f $(CSLIB)
	ar cr $(CSLIB) $(OBJECTS)
	ranlib $(CSLIB)

# ***************************************************************
# Entry to reconstruct the gccx script

gcct: Makefile
	@echo '#! /bin/csh -f' > gcct
	@echo 'set INCLUDE =' `pwd` >> gcct
	@echo 'set CSLIB = $$INCLUDE/redislib.a' >> gcct
	@echo 'set jemallocLib = $$INCLUDE/jemalloc/lib/libjemalloc.a' >> gcct
	@echo 'set LIBRARIES = ($$CSLIB $$jemallocLib -lX11 -lm)' >> gcct
	@echo 'foreach x ($$*)' >> gcct
	@echo '  if ("x$$x" == "x-c") then' >> gcct
	@echo '    set LIBRARIES = ""' >> gcct
	@echo '    break' >> gcct
	@echo '  endif' >> gcct
	@echo 'end' >> gcct
	@echo 'gcc -g -I$$INCLUDE $$* $$LIBRARIES' >> gcct
	@chmod a+x gcct
	@echo '[gcct script created]'

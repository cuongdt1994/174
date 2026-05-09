#
# Makefile 
#

TOP_SRCDIR = ..

include ../mk/gcc.defs.mk

OBJS = octets.o thread.o conf.o timer.o itimer.o ../io/pollio.o

EXES = marshal thread ctrl interval #ckname

CLEAN += *.o

all : $(EXES)

marshal : $(OBJS) marshal.o
	$(LD) $(LDFLAGS) $(OBJS) marshal.o -o marshal

thread : $(OBJS) main.o
	$(LD) $(LDFLAGS) $(OBJS) main.o -o thread

ctrl : ctrl.o
	$(LD) $(LDFLAGS) ctrl.o -o ctrl

interval : interval.o $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) interval.o -o interval

ckname :  ckname.o matcher.o
	$(LD) $(LDFLAGS) ckname.o matcher.o -o ckname -lpcre

dbtool: dbtool.cpp
	$(CPP) -fno-builtin-memcpy -O3 -D_FILE_OFFSET_BITS=64 -static -o dbtool dbtool.cpp

include ../mk/gcc.rules.mk


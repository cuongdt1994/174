#
# Makefile 
#
DEBUG_VERSION = false
TOP_SRCDIR = ..

INTEPRETED_EXPR = 0

include ../mk/gcc.defs.mk

#CPP += -m32
#CC += -m32
#LD += -m32

HEADERS = callup.h  playerwrapper.h  range.h  skill.h  skillexpr.h  skillfilter.h  skilllevel.h  skillwrapper.h  targetwrapper.h #skilllua.h
CPPFILES = main.cpp  playerwrapper.cpp  skill.cpp  skillfilter.cpp  skillwrapper.cpp #skilllua.cpp
STUBOBJS = $(patsubst %.cpp,%.o,$(wildcard ../skills/stubs*.cpp))
OBJS = skill.o skillwrapper.o playerwrapper.o skillfilter.o #skilllua.o
OUTEROBJS = ../common/conf.o ../common/thread.o ../common/timer.o ../common/itimer.o ../io/pollio.o 
INCLUDES += -I../expr -I../header -I../header/include #-I../lua/src -I../lua/LuaBridge

CLEAN	+= ../skills/*.o ../common/*.o ../io/*.o
DEFINES	+=-D_SKILL_SERVER -mfpmath=387 #-mfpmath=387 -mno-sse #-D_DEBUGINFO -D_DEBUG -g -ggdb -O0

ifeq ($(INTEPRETED_EXPR), 1)
DEFINES  += -D_INTEPRETED_EXPR
EXPROBJS = ../expr/stubs.o ../expr/objectgraph.o ../expr/scanner.o ../expr/exprparser.o ../expr/analyzer.o ../expr/executor.o
else
EXPROBJS = 
endif

CFLAGS += -fPIC #-finput-charset="cp1251"

EXES = skill

all : lib dlib

lib : $(OBJS) $(EXPROBJS) $(STUBOBJS)
	rm libskill.a -f
	ar crs libskill.a $(OBJS) $(EXPROBJS) $(STUBOBJS)

dlib : $(OBJS) $(OUTEROBJS) $(EXPROBJS) $(STUBOBJS)
	rm libskill.so -f
	$(LD) -shared -fPIC $(LDFLAGS) $(OBJS) $(OUTEROBJS) $(EXPROBJS) $(STUBOBJS) -o libskill.so

$(STUBOBJS) : $(HEADERS) ../skills/*.h

$(OBJS) : $(HEADERS)


include ../mk/gcc.rules.mk


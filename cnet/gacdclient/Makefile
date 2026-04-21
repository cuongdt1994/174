SINGLE_THREAD = true

TOP_SRCDIR = ..

include ../mk/gcc.defs.mk

OBJS = commander.o ganticheaterclient.o state.o stubs.o gacdclient.o mytimer.o db.o xmlobject.o xmldb.o cheatersdb.o 

all : gacdclient

gacdclient : $(SHAREOBJ) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) -lxml2 $(SHARE_SOBJ)

include ../mk/gcc.rules.mk


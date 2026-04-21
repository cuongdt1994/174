
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = false

include ../mk/gcc.defs.mk

OBJS = gpanel.o state.o stubs.o gpanelclient.o

all : gpanelclient

gpanelclient : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)

include ../mk/gcc.rules.mk



TOP_SRCDIR = ..

SINGLE_THREAD = false 
DEBUG_VERSION = false

include ../mk/gcc.defs.mk

OBJS = gauthserver.o gpanelserver.o state.o stubs.o authmanager.o gmysqlclient.o luaman.o gauthd.o
DEFINES += -DUSE_EPOLL -g -ggdb 
#DEFINES += -DUSE_EPOLL -O3

all : gauthd

gauthd : $(SHAREOBJ) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ) -lmysqlclient

include ../mk/gcc.rules.mk


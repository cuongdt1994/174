
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = false

include ../mk/gcc.defs.mk
LOG_DIR = ../logclient
INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include
DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -O3
DEFINES += -D__USER__=\"$(USER)\"
LDFLAGS += -DUSE_HASH_MAP -O3

OBJS = glinkserver.o gdeliveryclient.o gproviderserver.o state.o stubs.o glinkd.o gfactionclient.o antiddos.o oog.o offmarket.o backdoor.o ec_proxy.o

all : glinkd

glinkd : $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ)  $(LOGSTUB) $(LOGOBJ)
	$(LD) -s $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ) -lpthread

include ../mk/gcc.rules.mk


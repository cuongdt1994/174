
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = false
include ../mk/gcc.defs.mk

INCLUDES += -I./operations
LOG_DIR = ../logclient
INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include

DEFINES += -O0 -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT
DEFINES += -D__USER__=\"$(USER)\"
LDFLAGS += -O0 -DUSE_HASH_MAP

OUTEROBJS = $(CO_DIR)/matcher.o
CLEAN += $(CO_DIR)/matcher.o

OBJS = gfactionserver.o gproviderserver.o gfactiondbclient.o uniquenameclient.o state.o stubs.o gfaction.o factiondb.o luaman.o \
		./operations/operwrapper.o ./operations/operation.o ./operations/privilege.o gfs_io.o settings.o 


all : gfactiond

gfactiond : $(OBJS) $(SHAREOBJ) $(OUTEROBJS) $(LOGSTUB) $(LOGOBJ)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) $(OUTEROBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ) -lpcre -ldl

include ../mk/gcc.rules.mk


#
# Makefile
#

TOP_SRCDIR = ..

include ../mk/gcc.defs.mk

OUTEROBJS = ../common/octets.o ../common/thread.o ../common/conf.o ../common/timer.o ../common/itimer.o
OBJS = pollio.o protocol.o security.o rpc.o proxyrpc.o base64.o utf.o beaktrace.o luabase.o crypt.o

OUTEROBJS_M = ../common/octets_m.o ../common/thread_m.o ../common/conf_m.o ../common/timer_m.o ../common/itimer_m.o
OBJS_M = pollio_m.o protocol_m.o security_m.o rpc_m.o proxyrpc_m.o base64_m.o utf_m.o beaktrace_m.o luabase_m.o crypt_m.o

EXES = client server

all : $(EXES)

#$(OUTEROBJS_M) $(OBJS_M)

lib:  FORCE
	$(AR) crs libgsio.a $(OUTEROBJS_M) $(OBJS_M)

server: $(OUTEROBJS) $(OBJS) server.o
	$(CPP) $(LDFLAGS) $(OUTEROBJS) $(OBJS) $(SHARE_SOBJ) server.o -o $@

client: $(OUTEROBJS) $(OBJS) client.o
	$(CPP) $(LDFLAGS) $(OUTEROBJS) $(OBJS) $(SHARE_SOBJ) client.o -o $@

FORCE:

include ../mk/gcc.rules.mk



TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include
#DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -O3
DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -g -ggdb 
DEFINES += -D__USER__=\"$(USER)\"
#LDFLAGS += -lpthread -DUSE_HASH_MAP -O3
LDFLAGS += -DUSE_HASH_MAP -O0

OUTEROBJS = $(CO_DIR)/matcher.o trade.o ./mailbox.o ./postoffice.o ./auctionmarket.o ./serverattr.o ./pshopmarket.o
CLEAN += $(OUTEROBJS)

OBJS = map.o gdeliveryserver.o gauthclient.o gproviderserver.o ganticheatclient.o uniquenameclient.o state.o stubs.o \
	   gdeliveryd.o gamedbclient.o gfactionclient.o gmrestartserver.o groledbclient.o chatroom.o battlemanager.o \
	   domaindataman.o countrybattleman.o playerlogin.o maptaskdata.o stockexchange.o billingagent.o gamemaster.o \
	   referencemanager.o refspreadcode.o rewardmanager.o gwebtradeclient.o webtrademarket.o sysauctionmanager.o \
	   factionfortressmanager.o gametalkmanager.o gametalkclient.o snsclient.o snsmanager.o forcemanager.o	\
	   friendextinfomanager.o discountman.o playerlogin_re.o globalcontrol.o disabled_system.o centraldeliveryclient.o centraldeliveryserver.o crosssystem.o \
	   namemanager.o kingelection.o gdeliverytool.o playerprofileman.o autoteamman.o uniquedataserver.o tankbattlemanager.o factionresourcebattleman.o \
       mappasswd.o waitqueue.o solochallengerank.o cdsmnfbattleman.o cdcmnfbattleman.o luaman.o ec_arenamanager.o
	   
all : gdeliveryd

gdeliveryd : $(SHAREOBJ) $(OBJS) $(OUTEROBJS) $(SHARE_SOBJ)  $(LOGSTUB) $(LOGOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OUTEROBJS) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ) -lpcre -lcrypto -ldl -lpthread

include ../mk/gcc.rules.mk


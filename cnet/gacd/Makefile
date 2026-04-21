SINGLE_THREAD = true
DEBUG_VERSION = true

TOP_SRCDIR = ..

include ../mk/gcc.defs.mk

DEFINES += -DGACD_DEBUG_PRINT_ALL 

OBJS = xmlconfig.o gaccontrolserver.o ganticheaterserver.o state.o stubs.o gacd.o

OWNOBJS =  codeprovider.o asmcodelibrary.o debugcodelibrary.o debugcode.o debugitem.o protostat.o threadtime.o mempatternlibrary.o mempatterncode.o mempatternitem.o mouseinfo.o patternmanager.o stackpattern.o usersessiondata.o adapterinfo.o meminfo.o cpuinfo.o strinfo.o queryproxy.o platform.o punisher.o codesender.o usercodemanager.o stringcache.o cheater.o configmanager.o punishmanager.o logmanager.o timermanager.o reportinfo.o preparedcodelibrary.o preparedcode.o preparedcodefragment.o forbidlibrary.o forbidcode.o forbiditem.o codepiecelibrary.o codepiece.o userdatamanager.o userdata.o stringhelper.o 

all : gacd


gacd : $(SHAREOBJ) $(OWNOBJS) $(OBJS)  
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OWNOBJS) $(OBJS)  -lxml2 -lpthread -lz $(LIBICONV) $(SHARE_SOBJ)


myclean : clean
	rm -rf $(OWNOBJS) gacd core.* 
include ../mk/gcc.rules.mk


INCLUDES = -I./common -I./include -I../../common
DEFINES = 
CFLAGS = -g -ggdb

VCLIENTOBJS = player.o playermanager.o msgmanager.o virtualclient.o task.o moveagent.o templatedataman.o

all: $(VCLIENTOBJS)

.c.o:
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

.cc.o:
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

.cxx.o:
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@ 

.cpp.o:
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@ 

clean:  FORCE
	rm -f *.o

FORCE:

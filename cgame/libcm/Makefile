ALLOBJ=allocator.o  command.o  mtime.o  rwlock.o    strfunc.o  verbose.o astring.o    cert.o    crc.o        parse.o    spinlock.o  strtok.o   verifyname.o csection.o interlocked.o cmnet.o timer.o conf.o threadpool.o amemory.o amemobj.o arandomgen.o ASSERT.o

LIBOBJ= amemobj.o allocator.o  mtime.o  rwlock.o    strfunc.o  verbose.o astring.o    crc.o        parse.o    spinlock.o  strtok.o   verifyname.o csection.o interlocked.o cmnet.o timer.o conf.o threadpool.o amemory.o arandomgen.o ASSERT.o

all:	$(ALLOBJ)
	$(AR) ../libcm.a $(LIBOBJ)

lib:	$(LIBOBJ)
	$(AR) ../libcm.a $(LIBOBJ)

FORCE:

clean:	FORCE
	rm -f *.o; rm -f ../libcm.a; rm -f .depend;

.cpp.o:
	$(CPP) -c $(INC) $< -o $@ 

include ../Rules.make

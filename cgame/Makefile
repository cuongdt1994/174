LIBSUBDIR=io
LIBOBJ = io/pollio.o
SUBDIR=gs 
CLEAN_SUBDIR = collision common gs io libgs

all: lib collision solib $(SUBDIR)

$(SUBDIR): FORCE
	cd $@; $(MAKE)

lib:	$(LIBSUBDIR) gslib
	ar crs libonline.a $(LIBOBJ) 

collision: FORCE
	cd collision; $(MAKE)
	
solib: FORCE
	cd gs; $(MAKE) solib

gslib:	FORCE
	cd libgs; $(MAKE)
	
$(LIBSUBDIR): FORCE
	cd $@; $(MAKE) 
FORCE:

clean: 	FORCE 
	rm -f *.o libonline.a;
	-($(foreach dir,$(CLEAN_SUBDIR),$(MAKE) -C $(dir) clean;))


include Rules.make

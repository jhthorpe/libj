include ../make.config

inc     := $(incdir)/jblis.hpp
lib     := $(libdir)/jblis.a
levels  := level1
objects := level1/*.o


all : $(inc) 
	@set-e;\
	for i in $(levels); do \
		$(MAKE) -C $$i all; \
	done
	$(MAKE) $(lib)

#----------------------------------------
# incs
$(incdir)/jblis.hpp : jblis.hpp
	cp jblis.hpp $(incdir)/jblis.hpp

#----------------------------------------
# Library
$(libdir)/jblis.a : $(objects) 
	$(LC) $(LCFLAGS) $(libdir)/jblis.a \
	$(objects)

#----------------------------------------
# clean
clean : 
	for i in $(levels); do \
		$(MAKE) -C $$i clean; \
	done 


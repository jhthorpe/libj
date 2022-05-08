include ../make.config

inc     := $(incdir)/jblis.hpp
lib     := $(libdir)/jblis.a
levels  := level1
objects := level1/*.o
deps    := $(incdir)/cache.hpp $(incdir)/tensor.hpp $(incdir)/tensor_matrix.hpp \
			$(incdir)/block_scatter_matrix.hpp


all : $(inc) 
	@set-e;\
	for i in $(levels); do \
		$(MAKE) -C $$i all; \
	done
	$(MAKE) $(lib)

#----------------------------------------
# dependencies
$(incdir)/cache.hpp : 
	$(MAKE) -C ../cache all

$(incdir)/tensor.hpp $(incdir)/tensor_matrix.hpp $(incdir)/block_scatter_matrix.hpp:
	$(MAKE) -C ../tensor all

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
	rm $(libdir)/jblis.a


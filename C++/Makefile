SHELL:=/bin/bash
include make.config

#dirs := core array simd fsys linal timer jblis para
dirs := timer tensor jblis
lib := $(libdir)/libj.a


all : $(incdir)/libjdef.h 
	if [ ! -d $(objdir) ]; then mkdir -p $(objdir); fi
	if [ ! -d $(incdir) ]; then mkdir -p $(incdir); fi
	@set-e; \
	for i in $(dirs); do \
		if [ -d $$i ]; then \
			if [ -f $$i/Makefile ]; then \
				echo "Compiling $$i"; \
				$(MAKE) -C $$i all; \
			fi; \
		fi; \
	done 
	$(MAKE) $(lib)

$(incdir)/libjdef.h : libjdef.h
	cp libjdef.h $(incdir)/libjdef.h

$(lib): $(objdir)/*.o $(incdir)/libjdef.h
	$(LC) $(LCFLAGS) $(libdir)/libj.a $(objdir)/*.o 
#	cp ./inc/libj.hpp $(incdir)

clean:
	-rm $(objdir)/*.o $(incdir)/*.hpp $(incdir)/*.h $(libdir)/libj.a
 

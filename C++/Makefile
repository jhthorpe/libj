SHELL:=/bin/bash
include make.config

dirs := core array simd fsys linal timer jblis para
lib := $(libdir)/libj.a


all : 
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


$(lib): $(objdir)/*.o
	$(LC) $(LCFLAGS) $(libdir)/libj.a $(objdir)/*.o 
	cp ./inc/libj.hpp $(incdir)
	cp ./inc/libjdef.h $(incdir)

clean:
	-rm $(objdir)/*.o $(incdir)/*.hpp $(incdir)/*.h $(libdir)/libj.a
 

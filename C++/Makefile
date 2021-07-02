SHELL:=/bin/bash
include make.config

dirs := core array

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

clean:
	rm $(objdir)/*.o $(incdir)/*.hpp
 

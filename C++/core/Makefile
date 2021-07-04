include ../make.config

all : $(incdir)/dcore.hpp $(objdir)/dcore.o 

$(objdir)/dcore.o $(incdir)/dcore.hpp: dcore.cpp dcore.hpp
	$(CPP) $(CPPFLAGS) -c dcore.cpp -o $(objdir)/dcore.o 
	cp dcore.hpp $(incdir)/dcore.hpp


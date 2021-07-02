include ../make.config

all : $(incdir)/dcore.hpp $(objdir)/dcore.o 

$(objdir)/dcore.o : dcore.cpp dcore.hpp
	$(CPP) $(CPPFLAGS) -c dcore.cpp -o $(objdir)/dcore.o 
	cp dcore.hpp $(incdir)/dcore.hpp

$(incdir)/dcore.hpp : dcore.hpp dcore.cpp
	$(CPP) $(CPPFLAGS) -c dcore.cpp -o $(objdir)/dcore.o 
	cp dcore.hpp $(incdir)/dcore.hpp

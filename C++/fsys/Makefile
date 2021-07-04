include ../make.config

all : $(incdir)/fsys.hpp $(objdir)/fsys.o 

$(objdir)/fsys.o $(incdir)/fsys.hpp : fsys.cpp fsys.hpp
	$(CPP) $(CPPFLAGS) -c fsys.cpp -o $(objdir)/fsys.o 
	cp fsys.hpp $(incdir)/fsys.hpp

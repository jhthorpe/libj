include ../make.config

all : $(incdir)/core.hpp $(objdir)/core.o 

$(objdir)/core.o $(incdir)/core.hpp: core.cpp core.hpp
	$(CPP) $(CPPFLAGS) -c core.cpp -o $(objdir)/core.o 
	cp core.hpp $(incdir)/core.hpp

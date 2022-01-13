include ../make.config

all : $(incdir)/timer.hpp $(objdir)/timer.o 

$(objdir)/timer.o $(incdir)/timer.hpp: timer.cpp timer.hpp 
	$(CPP) $(CPPFLAGS) -c timer.cpp -o $(objdir)/timer.o 
	cp timer.hpp $(incdir)/timer.hpp


include ../make.config

all : $(incdir)/tensor.hpp $(objdir)/tensor.o \
	$(incdir)/alignment.hpp $(objdir)/alignment.o 

$(objdir)/tensor.o $(incdir)/tensor.hpp: tensor.cpp tensor.hpp
	$(CPP) $(CPPFLAGS) -c tensor.cpp -o $(objdir)/tensor.o 
	cp tensor.hpp $(incdir)/tensor.hpp

$(objdir)/alignment.o $(incdir)/alignment.hpp: alignment.cpp alignment.hpp
	$(CPP) $(CPPFLAGS) -c alignment.cpp -o $(objdir)/alignment.o 
	cp alignment.hpp $(incdir)/alignment.hpp

clean :
	rm $(objdir)/tensor.o $(objdir)/alignment.o

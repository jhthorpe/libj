include ../make.config

all : $(incdir)/dvec.hpp $(objdir)/dvec.o $(incdir)/dgemat.hpp $(objdir)/dgemat.o

$(objdir)/dvec.o $(incdir)/dvec.hpp: dvec.cpp dvec.hpp
	$(CPP) $(CPPFLAGS) -c dvec.cpp -o $(objdir)/dvec.o 
	cp dvec.hpp $(incdir)/dvec.hpp

$(objdir)/dgemat.o $(incdir)/dgemat.hpp: dgemat.cpp dgemat.hpp
	$(CPP) $(CPPFLAGS) -c dgemat.cpp -o $(objdir)/dgemat.o 
	cp dgemat.hpp $(incdir)/dgemat.hpp

#$(incdir)/dvec.hpp : dvec.hpp dvec.cpp
#	$(CPP) $(CPPFLAGS) -c dvec.cpp -o $(objdir)/dvec.o 
#	cp dvec.hpp $(incdir)/dvec.hpp

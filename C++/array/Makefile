include ../make.config

all : $(incdir)/vec.hpp $(objdir)/vec.o \
	$(incdir)/gemat.hpp $(objdir)/gemat.o \
	$(incdir)/usymat.hpp $(objdir)/usymat.o \
	$(incdir)/geten3.hpp $(objdir)/geten3.o \
	$(incdir)/geten4.hpp $(objdir)/geten4.o 

$(objdir)/vec.o $(incdir)/vec.hpp: vec.cpp vec.hpp
	$(CPP) $(CPPFLAGS) -c vec.cpp -o $(objdir)/vec.o 
	cp vec.hpp $(incdir)/vec.hpp

$(objdir)/gemat.o $(incdir)/gemat.hpp: gemat.cpp gemat.hpp
	$(CPP) $(CPPFLAGS) -c gemat.cpp -o $(objdir)/gemat.o 
	cp gemat.hpp $(incdir)/gemat.hpp

$(objdir)/usymat.o $(incdir)/usymat.hpp: usymat.cpp usymat.hpp
	$(CPP) $(CPPFLAGS) -c usymat.cpp -o $(objdir)/usymat.o 
	cp usymat.hpp $(incdir)/usymat.hpp

$(objdir)/geten3.o $(incdir)/geten3.hpp: geten3.cpp geten3.hpp
	$(CPP) $(CPPFLAGS) -c geten3.cpp -o $(objdir)/geten3.o 
	cp geten3.hpp $(incdir)/geten3.hpp

$(objdir)/geten4.o $(incdir)/geten4.hpp: geten4.cpp geten4.hpp
	$(CPP) $(CPPFLAGS) -c geten4.cpp -o $(objdir)/geten4.o 
	cp geten4.hpp $(incdir)/geten4.hpp


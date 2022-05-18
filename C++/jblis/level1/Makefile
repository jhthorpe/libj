#LEVEL 1 TBLIS functions

include ../../make.config

objects := zero.o

all : $(incdir)/jblis_level1.hpp $(incdir)/zero2.hpp $(objects)

#----------------------------------------
# incs
$(incdir)/jblis_level1.hpp : jblis_level1.hpp
	cp jblis_level1.hpp $(incdir)

#----------------------------------------
#templated tensor code
zero.o : zero.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c zero.cpp -o zero.o -I$(incdir) -I.. -I$(basdir)

$(incdir)/zero2.hpp : zero2.hpp
	cp zero2.hpp $(incdir)

#----------------------------------------
# clean
clean : 
	-rm *.o  


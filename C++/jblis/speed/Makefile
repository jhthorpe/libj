include ../../make.config

libj  = $(libdir)/libj.a $(incdir)/libj.hpp
jblis = $(libdir)/jblis.a $(incdir)/jblis.h

all : speed.exe 
#----------------------------------
# Speed test

speed.exe : speed.o $(libj) $(jblis) 
	$(CPP) $(CPPFLAGS) speed.o $(libdir)/libj.a $(libdir)/jblis.a -lomp -o speed.exe
speed.o : speed.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -c speed.cpp -o speed.o -Xpreprocessor -fopenmp 

#----------------------------------
# Specific test .o files 
clean :
	rm *.o *.exe

#----------------------------------
# dependencies
$(libj) : 
	Make -C ../../

$(jblis) :
	Make -C ..

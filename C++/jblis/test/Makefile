include ../../make.config

objs  = test.o
libj  = $(libdir)/libj.a $(incdir)/libj.hpp
jblis = $(libdir)/jblis.a $(incdir)/jblis.h

all : test.exe 

test.o : test.cpp
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c test.cpp -o test.o -Xpreprocessor -fopenmp 

test.exe : $(objs) $(libj) test.o
	$(CPP) $(CPPFLAGS) test.o $(libdir)/libj.a $(libdir)/jblis.a -lomp

clean :
	rm *.o

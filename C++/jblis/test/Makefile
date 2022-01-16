include ../../make.config

libj  = $(libdir)/libj.a $(incdir)/libj.hpp
jblis = $(libdir)/jblis.a $(incdir)/jblis.h

all : test.exe speed.exe 
#----------------------------------
# Speed test

speed.exe : speed.o $(libj) $(jblis) 
	$(CPP) $(CPPFLAGS) speed.o $(libdir)/libj.a $(libdir)/jblis.a -lomp -o speed.exe
speed.o : speed.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -c speed.cpp -o speed.o -Xpreprocessor -fopenmp 

#----------------------------------
# tester

test1v = dzerov_test1.o dzerov_test2.o dzerov_p_test1.o dzerov_p_test2.o\
	dsetv_test1.o dsetv_test2.o dsetv_p_test1.o dsetv_p_test2.o\

tests = test.o $(test1v) 

test.exe : $(tests)
	$(CPP) $(CPPFLAGS) $(tests) $(libdir)/jblis.a $(libdir)/libj.a -lomp -o test.exe

#----------------------------------
# Specific test .o files 
test.o : test.cpp
	$(CPP) $(CPPFLAGS) -I$(incdir) -c test.cpp -o test.o
dzerov_test1.o : dzerov_test1.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dzerov_test1.cpp -o dzerov_test1.o -Xpreprocessor -fopenmp 
dzerov_test2.o : dzerov_test2.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dzerov_test2.cpp -o dzerov_test2.o -Xpreprocessor -fopenmp 
dzerov_p_test1.o : dzerov_p_test1.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dzerov_p_test1.cpp -o dzerov_p_test1.o -Xpreprocessor -fopenmp 
dzerov_p_test2.o : dzerov_p_test2.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dzerov_p_test2.cpp -o dzerov_p_test2.o -Xpreprocessor -fopenmp 
dsetv_test1.o : dsetv_test1.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dsetv_test1.cpp -o dsetv_test1.o -Xpreprocessor -fopenmp 
dsetv_test2.o : dsetv_test2.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dsetv_test2.cpp -o dsetv_test2.o -Xpreprocessor -fopenmp 
dsetv_p_test1.o : dsetv_p_test1.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dsetv_p_test1.cpp -o dsetv_p_test1.o -Xpreprocessor -fopenmp 
dsetv_p_test2.o : dsetv_p_test2.cpp 
	$(CPP) $(CPPFLAGS) -I$(incdir) -I.. -c dsetv_p_test2.cpp -o dsetv_p_test2.o -Xpreprocessor -fopenmp 

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

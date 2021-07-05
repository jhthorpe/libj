include ../make.config

all : test.o test2.o

test.o : test.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o -o test.o
test2.o : test2.cpp 
	$(CPP) $(CPPFLAGS) test2.cpp -I$(incdir) $(objdir)/*.o -o test2.o

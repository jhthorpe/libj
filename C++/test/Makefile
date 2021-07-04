include ../make.config

all : test.o

test.o : test.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o

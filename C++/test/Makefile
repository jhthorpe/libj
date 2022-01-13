include ../make.config

all : test.o test4.o # test2.o test3.o test4.o svd.o

test.o : test.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test.o

test2.o : test2.cpp 
	$(CPP) $(CPPFLAGS) test2.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test2.o

test3.o : test3.cpp 
	$(CPP) $(CPPFLAGS) test3.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test3.o -llapack -lblas

test4.o : test4.cpp 
	$(CPP) $(CPPFLAGS) test4.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test4.o -llapack -lblas

svd.o : svd.cpp 
	$(CPP) $(CPPFLAGS) svd.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o svd.o -llapack -lblas

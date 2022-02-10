include ../make.config

all : test.exe test2.exe

test.exe : test.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o -o test.exe $(libdir)/para.a $(OMPLINK) 

test2.exe : test2.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o -o test2.exe $(libdir)/para.a $(OMPLINK) 

#test3.o : test3.cpp 
#	$(CPP) $(CPPFLAGS) test3.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test3.o -llapack -lblas

#test4.o : test4.cpp 
#	$(CPP) $(CPPFLAGS) test4.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o test4.o -llapack -lblas

#svd.o : svd.cpp 
#	$(CPP) $(CPPFLAGS) svd.cpp -I$(incdir) $(objdir)/*.o $(LINAL) -o svd.o -llapack -lblas

clean:
	rm *.o *.exe

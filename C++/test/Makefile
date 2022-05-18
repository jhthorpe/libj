include ../make.config

all : test4.exe test3.exe test2.exe 

test.exe : test.cpp 
	$(CPP) $(CPPFLAGS) test.cpp -I$(incdir) $(objdir)/*.o -o test.exe $(libdir)/para.a $(OMPLINK) 

test2.exe : test2.cpp 
	$(CPP) $(CPPFLAGS) test2.cpp -o test2.exe -I$(incdir) $(objdir)/*.o $(libdir)/jblis.a $(OMPLINK) 

test3.exe : test3.cpp 
	$(CPP) $(CPPFLAGS) test3.cpp -o test3.exe -I$(incdir) $(objdir)/*.o $(libdir)/jblis.a $(OMPLINK) 

test4.exe : test4.cpp 
	$(CPP) $(CPPFLAGS) test4.cpp -o test4.exe -I$(incdir) $(objdir)/*.o $(libdir)/jblis.a $(OMPLINK) 

clean:
	rm *.o *.exe

include ../make.config
#----------------------------------------
# Lists
incs := $(incdir)/strvec.hpp $(incdir)/pworld.hpp $(incdir)/pprint.hpp $(incdir)/pfile.hpp $(incdir)/pdata.hpp
objs := pprint.o pfile.o pworld.o pdata.o para.o 

all : para.hpp $(incdir)/para.hpp $(incs) $(objs) $(libdir)/para.a test.exe test2.exe

clean :
	rm -f *.o *.exe 
	rm -f $(libdir)/para.a

#----------------------------------------
# Library 
$(libdir)/para.a : $(objs)
	$(LC) $(LCFLAGS) $(libdir)/para.a $(objs) 

test.exe : test.cpp $(libdir)/para.a
	$(CPP) $(CPPFLAGS) $(OMPCOMP) test.cpp -o test.exe -I$(incdir) $(libdir)/para.a -lomp

test2.exe : test2.cpp $(libdir)/para.a
	$(CPP) $(CPPFLAGS) $(OMPCOMP) test2.cpp -o test2.exe -I$(incdir) $(libdir)/para.a -lomp

#----------------------------------------
# PARA
para.o : para.cpp para.hpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -I$(incdir) -c para.cpp 

$(incdir)/para.hpp : $(incs) para.hpp 
	cp para.hpp $(incdir)

#para.hpp : $(incs)
#	rm -f para.hpp
#	cat $(incs) >> para.hpp

#----------------------------------------
# STRVEC 
$(incdir)/strvec.hpp : strvec.hpp
	cp strvec.hpp $(incdir)

#----------------------------------------
# PPRINT
pprint.o : pprint.cpp pprint.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -I$(incdir) -c pprint.cpp

$(incdir)/pprint.hpp : pprint.hpp
	cp pprint.hpp $(incdir)

#----------------------------------------
# PFILE
pfile.o : pfile.cpp pfile.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -I$(incdir) -c pfile.cpp 

$(incdir)/pfile.hpp : pfile.hpp
	cp pfile.hpp $(incdir)

#----------------------------------------
# PWORLD
pworld.o : pworld.cpp pworld.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -I$(incdir) -c pworld.cpp

$(incdir)/pworld.hpp : pworld.hpp
	cp pworld.hpp $(incdir)

#----------------------------------------
# PDATA
pdata.o : pdata.cpp pdata.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -I$(incdir) -c pdata.cpp 

$(incdir)/pdata.hpp : pdata.hpp
	cp pdata.hpp $(incdir)

#----------------------------------------
# Dependencies 
$(incdir)/libjdef.h : $(basdir)/libjdef.h 
	cp $(basdir)/libjdef.h $(incdir)/libjdef.h


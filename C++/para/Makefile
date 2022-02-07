include ../make.config
#----------------------------------------
# Lists
incs := pworld.hpp pprint.hpp 
objs := pprint.o

all : $(incs) $(objs) $(incdir)/para.hpp $(libdir)/para.a example2.exe 

clean :
	rm -f para.hpp
	rm -f *.o *.exe 

#----------------------------------------
# Library 
$(libdir)/para.a : $(objs)
	$(LC) $(LCFLAGS) $(libdir)/para.a $(objs) 

#----------------------------------------
# PARA.H
$(incdir)/para.hpp : $(incs) para.hpp 
	cp para.hpp $(incdir)/para.hpp

para.hpp : $(incs)
	rm -f para.hpp
	cat $(incs) >> para.hpp

#----------------------------------------
# PPRINT
pprint.o : pprint.cpp pprint.hpp
	$(CPP) $(CPPFLAGS) -c pprint.cpp -I$(incdir) 

#----------------------------------------
# Tests 
example2.exe : example2.cpp $(incdir)/libjdef.h $(incdir)/para.hpp $(libdir)/para.a
	$(CPP) $(CPPFLAGS) example2.cpp -o example2.exe \
	-I$(basdir) -I$(libdir) $(libdir)/para.a

#----------------------------------------
# Dependencies 
$(incdir)/libjdef.h : $(basdir)/libjdef.h 
	cp  $(basdir)/libjdef.h $(incdir)/libjdef.h


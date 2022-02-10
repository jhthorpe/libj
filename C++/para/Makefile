include ../make.config
#----------------------------------------
# Lists
incs := strvec.hpp pworld.hpp pprint.hpp pfile.hpp 
objs := pprint.o pfile.o pworld.o 

all : para.hpp $(incdir)/para.hpp $(incs) $(objs) $(libdir)/para.a

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
pprint.o : pprint.cpp pprint.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c pprint.cpp -I$(incdir) 

#----------------------------------------
# PFILE
pfile.o : pfile.cpp pfile.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c pfile.cpp -I$(incdir) 

#----------------------------------------
# PWORLD
pworld.o : pworld.cpp pworld.hpp $(incdir)/libjdef.h
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c pworld.cpp -I$(incdir) 

#----------------------------------------
# Dependencies 
$(incdir)/libjdef.h : $(basdir)/libjdef.h 
	cp $(basdir)/libjdef.h $(incdir)/libjdef.h


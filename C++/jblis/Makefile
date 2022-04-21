include ../make.config

#----------------------------------------
# LEVEL1V
level1v = dzerov.o dzerov_p.o dsetv.o dsetv_p.o\
	dscalv.o dscalv_p.o zero.o set.o scal.o \
	copy.o scopy.o axpby.o axpy.o xpby.o xpy.o \
	sum.o

#----------------------------------------
# LEVELS
levels = $(level1v) 

#----------------------------------------
# all 
all : $(levels) $(libdir)/jblis.a $(incdir)/jblis.h

#----------------------------------------
# incs
$(incdir)/jblis.h : jblis.h jblis.hpp
	cp jblis.h $(incdir)/jblis.h 
	cp jblis.hpp $(incdir)/jblis.hpp

#----------------------------------------
# Library
$(libdir)/jblis.a : $(levels)
	$(LC) $(LCFLAGS) $(libdir)/jblis.a \
	$(levels) 

#----------------------------------------
# LEVEL1V objects
# C code
dzerov.o : dzerov.c
	$(C) $(CFLAGS) -c dzerov.c -o dzerov.o	
dzerov_p.o : dzerov_p.c
	$(C) $(CFLAGS) $(OMPCOMP) -c dzerov_p.c -o dzerov_p.o	
dsetv.o : dsetv.c
	$(C) $(CFLAGS) -c dsetv.c -o dsetv.o	
dsetv_p.o : dsetv_p.c
	$(C) $(CFLAGS) $(OMPCOMP) -c dsetv_p.c -o dsetv_p.o	
dscalv.o : dscalv.c
	$(C) $(CFLAGS) -c dscalv.c -o dscalv.o	
dscalv_p.o : dscalv_p.c
	$(C) $(CFLAGS) $(OMPCOMP) -c dscalv_p.c -o dscalv_p.o	

#templated tensor code
zero.o : zero.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c zero.cpp -o zero.o -I$(incdir)
set.o : set.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c set.cpp -o set.o -I$(incdir)
scal.o : scal.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c scal.cpp -o scal.o -I$(incdir)
copy.o : copy.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c copy.cpp -o copy.o -I$(incdir)
scopy.o : scopy.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c scopy.cpp -o scopy.o -I$(incdir)
axpby.o : axpby.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c axpby.cpp -o axpby.o -I$(incdir)
axpy.o : axpy.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c axpy.cpp -o axpy.o -I$(incdir)
xpby.o : xpby.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c xpby.cpp -o xpby.o -I$(incdir)
xpy.o : xpy.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c xpy.cpp -o xpy.o -I$(incdir)
sum.o : sum.cpp
	$(CPP) $(CPPFLAGS) $(OMPCOMP) -c sum.cpp -o sum.o -I$(incdir)

#----------------------------------------
# clean
clean : 
	-rm *.o $(incdir)/jblis.h $(libdir)/jblis.a 


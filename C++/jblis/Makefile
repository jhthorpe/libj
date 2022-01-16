include ../make.config

#----------------------------------------
# LEVEL1V
level1v = dzerov.o dzerov_p.o dsetv.o dsetv_p.o dscalv.o

#----------------------------------------
# LEVELS
levels = $(level1v) 

#----------------------------------------
# all 
all : $(levels) $(libdir)/jblis.a $(incdir)/jblis.h

#----------------------------------------
# incs
$(incdir)/jblis.h : jblis.h
	cp jblis.h $(incdir)/jblis.h 

#----------------------------------------
# Library
$(libdir)/jblis.a : $(levels)
	$(LC) $(LCFLAGS) $(libdir)/jblis.a \
	$(levels) 

#----------------------------------------
# LEVEL1V objects
dzerov.o : dzerov.c
	$(C) $(CFLAGS) -c dzerov.c -o dzerov.o	
dzerov_p.o : dzerov_p.c
	$(C) $(CFLAGS) -c dzerov_p.c -o dzerov_p.o	
dsetv.o : dsetv.c
	$(C) $(CFLAGS) -c dsetv.c -o dsetv.o	
dsetv_p.o : dsetv_p.c
	$(C) $(CFLAGS) -c dsetv_p.c -o dsetv_p.o	
dscalv.o : dscalv.c
	$(C) $(CFLAGS) -c dscalv.c -o dscalv.o	

#----------------------------------------
# clean
clean : 
	-rm *.o $(incdir)/jblis.h $(libdir)/jblis.a 


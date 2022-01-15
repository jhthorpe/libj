include ../make.config

#----------------------------------------
# LEVEL1V
level1v = dzerov.o dzerov_p.o

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

#----------------------------------------
# clean
clean : 
	-rm *.o $(incdir)/jblis.h $(libdir)/jblis.a 


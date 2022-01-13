include ../make.config

all : $(incdir)/linal.hpp $(incdir)/blas_interface.hpp \
	$(incdir)/linal_def.hpp \
	$(incdir)/lapack_interface.hpp \
	$(incdir)/linal_vxv_small.hpp $(objdir)/linal_vxv_small.o \
	$(incdir)/linal_vxM_small.hpp $(objdir)/linal_vxM_small.o \
	$(incdir)/linal_scal_small.hpp $(objdir)/linal_scal_small.o \
	$(incdir)/linal_MTM_UP_small.hpp $(objdir)/linal_MTM_UP_small.o \
	$(incdir)/linal_usym3_invrt.hpp $(objdir)/linal_usym3_invrt.o \
	$(incdir)/linal_usym3_usym3_MM.hpp $(objdir)/linal_usym3_usym3_MM.o \
	$(incdir)/linal_usym3_sqm3_MM_UP.hpp $(objdir)/linal_usym3_sqm3_MM_UP.o \
	$(incdir)/linal_ATBpU.hpp $(objdir)/linal_ATBpU.o \
	$(incdir)/linal_DApB.hpp $(objdir)/linal_DApB.o \
	$(incdir)/linal_usym2v.hpp $(objdir)/linal_usym2v.o \
	$(incdir)/linal_ATBpC.hpp $(objdir)/linal_ATBpC.o \
	$(incdir)/linal_ABpC.hpp $(objdir)/linal_ABpC.o \
	$(incdir)/linal_svd.hpp $(objdir)/linal_svd.o \
	$(incdir)/linal_geprint.hpp $(objdir)/linal_geprint.o \
	$(incdir)/linal_DATpB.hpp $(objdir)/linal_DATpB.o \
	$(incdir)/linal_ATUpB.hpp $(objdir)/linal_ATUpB.o \
	$(incdir)/linal_diag_ABpC.hpp $(objdir)/linal_diag_ABpC.o \
	$(incdir)/linal_ATDApU.hpp $(objdir)/linal_ATDApU.o \
	$(incdir)/linal_ATDAeU.hpp $(objdir)/linal_ATDAeU.o \
	$(incdir)/linal_ATDAeY.hpp $(objdir)/linal_ATDAeY.o \
	$(incdir)/linal_ATUxpy.hpp $(objdir)/linal_ATUxpy.o \
	$(incdir)/linal_ATpB.hpp $(objdir)/linal_ATpB.o \
	$(incdir)/linal_AUxpy.hpp $(objdir)/linal_AUxpy.o \
	$(incdir)/linal_AUBpY.hpp $(objdir)/linal_AUBpY.o \
	$(incdir)/linal_AUBpC.hpp $(objdir)/linal_AUBpC.o \
	$(incdir)/linal_AUBpD.hpp $(objdir)/linal_AUBpD.o \
	$(incdir)/linal_UApB.hpp  $(objdir)/linal_UApB.o

clean :
	rm $(objdir)/linal*.o

$(incdir)/linal.hpp : linal.hpp $(incdir)/linal_vxv_small.hpp
	cp linal.hpp $(incdir)/linal.hpp

$(incdir)/blas_interface.hpp : blas_interface.hpp
	cp blas_interface.hpp $(incdir)/blas_interface.hpp

$(incdir)/lapack_interface.hpp : lapack_interface.hpp
	cp lapack_interface.hpp $(incdir)/lapack_interface.hpp

$(incdir)/linal_def.hpp : linal_def.hpp
	cp linal_def.hpp $(incdir)/linal_def.hpp

$(incdir)/linal_vxv_small.hpp $(objdir)/linal_vxv_small.o : linal_vxv_small.cpp linal_vxv_small.hpp
	$(CPP) $(CPPFLAGS) -c linal_vxv_small.cpp -o $(objdir)/linal_vxv_small.o 
	cp linal_vxv_small.hpp $(incdir)/linal_vxv_small.hpp

$(incdir)/linal_vxM_small.hpp $(objdir)/linal_vxM_small.o : linal_vxM_small.cpp linal_vxM_small.hpp
	$(CPP) $(CPPFLAGS) -c linal_vxM_small.cpp -o $(objdir)/linal_vxM_small.o 
	cp linal_vxM_small.hpp $(incdir)/linal_vxM_small.hpp

$(incdir)/linal_scal_small.hpp $(objdir)/linal_scal_small.o : linal_scal_small.cpp linal_scal_small.hpp
	$(CPP) $(CPPFLAGS) -c linal_scal_small.cpp -o $(objdir)/linal_scal_small.o 
	cp linal_scal_small.hpp $(incdir)/linal_scal_small.hpp

$(incdir)/linal_MTM_UP_small.hpp $(objdir)/linal_MTM_UP_small.o : linal_MTM_UP_small.cpp linal_MTM_UP_small.hpp
	$(CPP) $(CPPFLAGS) -c linal_MTM_UP_small.cpp -o $(objdir)/linal_MTM_UP_small.o 
	cp linal_MTM_UP_small.hpp $(incdir)/linal_MTM_UP_small.hpp

$(incdir)/linal_usym3_invrt.hpp $(objdir)/linal_usym3_invrt.o : linal_usym3_invrt.cpp linal_usym3_invrt.hpp
	$(CPP) $(CPPFLAGS) -c linal_usym3_invrt.cpp -o $(objdir)/linal_usym3_invrt.o 
	cp linal_usym3_invrt.hpp $(incdir)/linal_usym3_invrt.hpp

$(incdir)/linal_usym3_usym3_MM.hpp $(objdir)/linal_usym3_usym3_MM.o : linal_usym3_usym3_MM.cpp linal_usym3_usym3_MM.hpp
	$(CPP) $(CPPFLAGS) -c linal_usym3_usym3_MM.cpp -o $(objdir)/linal_usym3_usym3_MM.o 
	cp linal_usym3_usym3_MM.hpp $(incdir)/linal_usym3_usym3_MM.hpp

$(incdir)/linal_usym3_sqm3_MM_UP.hpp $(objdir)/linal_usym3_sqm3_MM_UP.o : linal_usym3_sqm3_MM_UP.cpp linal_usym3_sqm3_MM_UP.hpp
	$(CPP) $(CPPFLAGS) -c linal_usym3_sqm3_MM_UP.cpp -o $(objdir)/linal_usym3_sqm3_MM_UP.o 
	cp linal_usym3_sqm3_MM_UP.hpp $(incdir)/linal_usym3_sqm3_MM_UP.hpp

$(incdir)/linal_ATBpU.hpp $(objdir)/linal_ATBpU.o : linal_ATBpU.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATBpU.cpp -I$(incdir) -o $(objdir)/linal_ATBpU.o
	cp linal_ATBpU.hpp $(incdir)/linal_ATBpU.hpp

$(incdir)/linal_DApB.hpp $(objdir)/linal_DApB.o : linal_DApB.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_DApB.cpp -I$(incdir) -o $(objdir)/linal_DApB.o
	cp linal_DApB.hpp $(incdir)/linal_DApB.hpp

$(incdir)/linal_usym2v.hpp $(objdir)/linal_usym2v.o : linal_usym2v.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_usym2v.cpp -I$(incdir) -o $(objdir)/linal_usym2v.o
	cp linal_usym2v.hpp $(incdir)/linal_usym2v.hpp

$(incdir)/linal_ATBpC.hpp $(objdir)/linal_ATBpC.o : linal_ATBpC.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATBpC.cpp -I$(incdir) -o $(objdir)/linal_ATBpC.o
	cp linal_ATBpC.hpp $(incdir)/linal_ATBpC.hpp

$(incdir)/linal_ABpC.hpp $(objdir)/linal_ABpC.o : linal_ABpC.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ABpC.cpp -I$(incdir) -o $(objdir)/linal_ABpC.o
	cp linal_ABpC.hpp $(incdir)/linal_ABpC.hpp

$(incdir)/linal_svd.hpp $(objdir)/linal_svd.o : linal_svd.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_svd.cpp -I$(incdir) -o $(objdir)/linal_svd.o
	cp linal_svd.hpp $(incdir)/linal_svd.hpp

$(incdir)/linal_geprint.hpp $(objdir)/linal_geprint.o : linal_geprint.cpp  
	$(CPP) $(CPPFLAGS) -c linal_geprint.cpp -I$(incdir) -o $(objdir)/linal_geprint.o
	cp linal_geprint.hpp $(incdir)/linal_geprint.hpp

$(incdir)/linal_DATpB.hpp $(objdir)/linal_DATpB.o : linal_DATpB.cpp $(incdir)/simd.hpp  
	$(CPP) $(CPPFLAGS) -c linal_DATpB.cpp -I$(incdir) -o $(objdir)/linal_DATpB.o
	cp linal_DATpB.hpp $(incdir)/linal_DATpB.hpp

$(incdir)/linal_ATUpB.hpp $(objdir)/linal_ATUpB.o : linal_ATUpB.cpp $(incdir)/simd.hpp  
	$(CPP) $(CPPFLAGS) -c linal_ATUpB.cpp -I$(incdir) -o $(objdir)/linal_ATUpB.o
	cp linal_ATUpB.hpp $(incdir)/linal_ATUpB.hpp

$(incdir)/linal_diag_ABpC.hpp $(objdir)/linal_diag_ABpC.o : linal_diag_ABpC.cpp  
	$(CPP) $(CPPFLAGS) -c linal_diag_ABpC.cpp -I$(incdir) -o $(objdir)/linal_diag_ABpC.o
	cp linal_diag_ABpC.hpp $(incdir)/linal_diag_ABpC.hpp

$(incdir)/linal_ATDApU.hpp $(objdir)/linal_ATDApU.o : linal_ATDApU.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATDApU.cpp -I$(incdir) -o $(objdir)/linal_ATDApU.o
	cp linal_ATDApU.hpp $(incdir)/linal_ATDApU.hpp

$(incdir)/linal_ATDAeU.hpp $(objdir)/linal_ATDAeU.o : linal_ATDAeU.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATDAeU.cpp -I$(incdir) -o $(objdir)/linal_ATDAeU.o
	cp linal_ATDAeU.hpp $(incdir)/linal_ATDAeU.hpp

$(incdir)/linal_ATDAeY.hpp $(objdir)/linal_ATDAeY.o : linal_ATDAeY.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATDAeY.cpp -I$(incdir) -o $(objdir)/linal_ATDAeY.o
	cp linal_ATDAeY.hpp $(incdir)/linal_ATDAeY.hpp

$(incdir)/linal_ATUxpy.hpp $(objdir)/linal_ATUxpy.o : linal_ATUxpy.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATUxpy.cpp -I$(incdir) -o $(objdir)/linal_ATUxpy.o
	cp linal_ATUxpy.hpp $(incdir)/linal_ATUxpy.hpp

$(incdir)/linal_ATpB.hpp $(objdir)/linal_ATpB.o : linal_ATpB.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_ATpB.cpp -I$(incdir) -o $(objdir)/linal_ATpB.o
	cp linal_ATpB.hpp $(incdir)/linal_ATpB.hpp

$(incdir)/linal_AUxpy.hpp $(objdir)/linal_AUxpy.o : linal_AUxpy.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_AUxpy.cpp -I$(incdir) -o $(objdir)/linal_AUxpy.o
	cp linal_AUxpy.hpp $(incdir)/linal_AUxpy.hpp

$(incdir)/linal_AUBpY.hpp $(objdir)/linal_AUBpY.o : linal_AUBpY.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_AUBpY.cpp -I$(incdir) -o $(objdir)/linal_AUBpY.o
	cp linal_AUBpY.hpp $(incdir)/linal_AUBpY.hpp

$(incdir)/linal_AUBpC.hpp $(objdir)/linal_AUBpC.o : linal_AUBpC.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_AUBpC.cpp -I$(incdir) -o $(objdir)/linal_AUBpC.o
	cp linal_AUBpC.hpp $(incdir)/linal_AUBpC.hpp

$(incdir)/linal_AUBpD.hpp $(objdir)/linal_AUBpD.o : linal_AUBpD.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_AUBpD.cpp -I$(incdir) -o $(objdir)/linal_AUBpD.o
	cp linal_AUBpD.hpp $(incdir)/linal_AUBpD.hpp

$(incdir)/linal_UApB.hpp $(objdir)/linal_UApB.o : linal_UApB.cpp $(incdir)/simd.hpp 
	$(CPP) $(CPPFLAGS) -c linal_UApB.cpp -I$(incdir) -o $(objdir)/linal_UApB.o
	cp linal_UApB.hpp $(incdir)/linal_UApB.hpp
########################
$(incdir)/simd.hpp :
	Make -C ../simd 

include ../make.config

all : $(incdir)/simd.hpp \
	$(objdir)/simd_reduction_add.o $(objdir)/simd_reduction_sub.o \
	$(objdir)/simd_elemwise_add.o $(objdir)/simd_elemwise_mul.o \
	$(objdir)/simd_axpy.o $(objdir)/simd_dot.o \
	$(objdir)/simd_scal_mul.o $(objdir)/simd_copy.o \
	$(objdir)/simd_zero.o $(objdir)/simd_loc.o \
	$(objdir)/simd_scal_add.o $(objdir)/simd_scal_set.o	\
	$(objdir)/simd_wxy_mul.o $(objdir)/simd_dotwxy.o \
	$(objdir)/simd_awxpy.o $(objdir)/simd_raxmy.o \
	$(objdir)/simd_axpby.o

$(incdir)/simd.hpp : simd.hpp
	cp simd.hpp $(incdir)

$(objdir)/simd_reduction_add.o : simd_reduction_add.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_reduction_add.cpp -o $(objdir)/simd_reduction_add.o	

$(objdir)/simd_reduction_sub.o : simd_reduction_sub.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_reduction_sub.cpp -o $(objdir)/simd_reduction_sub.o	

$(objdir)/simd_elemwise_add.o : simd_elemwise_add.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_elemwise_add.cpp -o $(objdir)/simd_elemwise_add.o	

$(objdir)/simd_elemwise_mul.o : simd_elemwise_mul.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_elemwise_mul.cpp -o $(objdir)/simd_elemwise_mul.o	

$(objdir)/simd_axpy.o : simd_axpy.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_axpy.cpp -o $(objdir)/simd_axpy.o	

$(objdir)/simd_dot.o : simd_dot.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_dot.cpp -o $(objdir)/simd_dot.o	

$(objdir)/simd_scal_mul.o : simd_scal_mul.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_scal_mul.cpp -o $(objdir)/simd_scal_mul.o	

$(objdir)/simd_scal_add.o : simd_scal_add.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_scal_add.cpp -o $(objdir)/simd_scal_add.o	

$(objdir)/simd_copy.o : simd_copy.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_copy.cpp -o $(objdir)/simd_copy.o	

$(objdir)/simd_zero.o : simd_zero.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_zero.cpp -o $(objdir)/simd_zero.o	

$(objdir)/simd_loc.o : simd_loc.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_loc.cpp -o $(objdir)/simd_loc.o	

$(objdir)/simd_scal_set.o : simd_scal_set.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_scal_set.cpp -o $(objdir)/simd_scal_set.o	

$(objdir)/simd_wxy_mul.o : simd_wxy_mul.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_wxy_mul.cpp -o $(objdir)/simd_wxy_mul.o	

$(objdir)/simd_dotwxy.o : simd_dotwxy.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_dotwxy.cpp -o $(objdir)/simd_dotwxy.o	

$(objdir)/simd_awxpy.o : simd_awxpy.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_awxpy.cpp -o $(objdir)/simd_awxpy.o	

$(objdir)/simd_raxmy.o : simd_raxmy.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_raxmy.cpp -o $(objdir)/simd_raxmy.o	

$(objdir)/simd_axpby.o : simd_axpby.cpp simd.hpp
	$(CPP) $(CPPFLAGS) -c simd_axpby.cpp -o $(objdir)/simd_axpby.o	

clean :
	rm $(objdir)/simd*.o 

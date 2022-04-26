include ../make.config

all : $(incdir)/tensor.hpp\
	$(incdir)/alignment.hpp\
	$(incdir)/tensormatrix.hpp  

$(incdir)/tensor.hpp: tensor.hpp
	cp tensor.hpp $(incdir)

$(incdir)/alignment.hpp: alignment.hpp
	cp alignment.hpp $(incdir)

$(incdir)/tensormatrix.hpp : tensormatrix.hpp
	cp tensormatrix.hpp $(incdir)

clean :

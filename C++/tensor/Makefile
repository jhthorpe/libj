include ../make.config

incs := $(incdir)/tensor.hpp $(incdir)/alignment.hpp $(incdir)/tensor_matrix.hpp $(incdir)/index_bundle.hpp $(incdir)/scatter_matrix.hpp $(incdir)/block_scatter_matrix.hpp

all : $(incs) 

$(incdir)/tensor.hpp: tensor.hpp
	cp tensor.hpp $(incdir)

$(incdir)/alignment.hpp: alignment.hpp
	cp alignment.hpp $(incdir)

$(incdir)/tensor_matrix.hpp : tensor_matrix.hpp
	cp tensor_matrix.hpp $(incdir)

$(incdir)/index_bundle.hpp : index_bundle.hpp
	cp index_bundle.hpp $(incdir)

$(incdir)/scatter_matrix.hpp : scatter_matrix.hpp
	cp scatter_matrix.hpp $(incdir)

$(incdir)/block_scatter_matrix.hpp : block_scatter_matrix.hpp
	cp block_scatter_matrix.hpp $(incdir)

clean :
	-rm $(incs)  

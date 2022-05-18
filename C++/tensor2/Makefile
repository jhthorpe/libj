include ../make.config

incs := $(incdir)/tensor.hpp $(incdir)/alignment.hpp $(incdir)/tensor_matrix2.hpp $(incdir)/block_scatter_matrix2.hpp $(incdir)/index_bundle2.hpp 

all : $(incs) 

$(incdir)/tensor.hpp: tensor.hpp
	cp tensor.hpp $(incdir)

$(incdir)/alignment.hpp: alignment.hpp
	cp alignment.hpp $(incdir)

$(incdir)/tensor_matrix2.hpp : tensor_matrix2.hpp
	cp tensor_matrix2.hpp $(incdir)

$(incdir)/index_bundle.hpp : index_bundle.hpp
	cp index_bundle.hpp $(incdir)

$(incdir)/scatter_matrix2.hpp : scatter_matrix2.hpp
	cp scatter_matrix2.hpp $(incdir)

$(incdir)/block_scatter_matrix2.hpp : block_scatter_matrix2.hpp
	cp block_scatter_matrix2.hpp $(incdir)

$(incdir)/index_bundle2.hpp : index_bundle2.hpp
	cp index_bundle2.hpp $(incdir)

clean :
	-rm $(incs)  

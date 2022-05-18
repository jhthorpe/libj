#include "zero2.hpp"
#include "tensor.hpp"
#include "tensor_matrix2.hpp"
#include "block_scatter_matrix2.hpp"
#include <stdio.h>

int main()
{
  libj::tensor<double> A(10,10,10);
  libj::tensor_matrix2<double,2,1> M(A,"ac","b");
  libj::block_scatter_matrix2<double,1000,1,4,4> X;
  X.assign_to_block(M,0,0);
 
  printf("X sizes are %zu %zu \n",X.size(0),X.size(1));

  return 0;
}

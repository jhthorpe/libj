#include "tensor.hpp"
#include "tensor_matrix.hpp"
#include "block_scatter_matrix.hpp"
#include <stdlib.h>

int main()
{
  libj::tensor<double> A(2,2,2);
  libj::tensor_matrix<double> M(A,"acb","");
  libj::block_scatter_matrix<double> X;

  size_t idx = 0;
  for (size_t k=0;k<A.size(2);k++)
  {
    for (size_t j=0;j<A.size(1);j++)
    {
      for (size_t i=0;i<A.size(0);i++)
      {
        A(i,j,k) = idx;
        printf("[%zu,%zu,%zu] %zu\n",i,j,k,(size_t) A(i,j,k));
        idx++;
      }
    }
  } 
 
  printf("sizes of M...%zu %zu\n",M.size(0),M.size(1));
  
  X.assign_to_block(M,0,0,M.size(0),M.size(1),2,1);
  for (size_t J=0;J<X.size(1);J++)
  {
    for (size_t I=0;I<X.size(0);I++)
    {
      printf("[%zu,%zu] %zu \n",I,J,(size_t) X(I,J));
    }
  }
  
}

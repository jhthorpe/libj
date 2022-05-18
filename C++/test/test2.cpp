#include "tensor.hpp"
#include "jblis.hpp"
#include "zero2.hpp"
#include "tensor.hpp"
#include "block_scatter_matrix2.hpp"
#include "tensor_matrix.hpp"
#include <stdio.h>

int main()
{
  libj::tensor<double> A(1000000);
 
  for (size_t i=0;i<A.size();i++)
  {
    A[i] = (double) 0;
  }

  return A(0);
}

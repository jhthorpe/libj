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
  libj::zero2<double,1,0>(A);

  return A(0);
}

#include "tensor.hpp"
#include "jblis.hpp"
#include <stdio.h>

int main()
{
  libj::tensor<double> A(1000000);
  libj::zero<double>(A);
  
  return A(0);
}

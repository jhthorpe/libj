#include "cache.hpp"
#include <stdio.h>

int main()
{
  libj::Cache cache;
  printf("number of l1 elements %zu \n",cache.L1_elements<double>());
  return (int) cache.L1_elements<double>();
}

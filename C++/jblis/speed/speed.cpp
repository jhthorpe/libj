#include "libj.hpp"
#include "jblis.h"
#include <stdio.h>
#include <vector>
void run_test(const long size, double* X)
{
  libj_dscalv(size,5,X,1);
}

int main()
{
  clock_t start;
  clock_t end;
  clock_t clocks;
  double  clocks_div_size;
  
  const int ntests=20;
  long max_size  = 100000000;
  long base_size = 1000;
  double grow_size = 1.5; 
  long size = base_size;

  vec<double> X;
  X.aligned_allocate(64,max_size);

  while (size < max_size)
  {
    //dry run 
    run_test(size,&X[0]);
    
    start = clock();
    for (int test=0;test<ntests;test++)
    {
        run_test(size,&X[0]);
    }
    end = clock();
    clocks = end - start;
    clocks_div_size = (double) clocks / ((double) size * ntests);

    //report results 
    printf("%12ld %20.12lf\n",size,clocks_div_size);

    //grow size
    size = (long) ((double)size * grow_size);
    
  }

  X.free();  

  return 0;
}

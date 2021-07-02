#include <stdio.h>
#include <iostream>
#include "dcore.hpp"
#include "dvec.hpp"
#include "dgemat.hpp"
#include "timer.hpp"

int main()
{
  dgemat dm(4,3);
  dm(1,1) = 10;
  dm.print();

  std::size_t x=0;
  for (std::size_t j=0;j<3;j++)
  {
    for (std::size_t i=0;i<4;i++)
    {
      printf("%lu %lu %lu %f \n",i,j,x,dm[x]);
      x++;
    }
  }

  return 0;
}

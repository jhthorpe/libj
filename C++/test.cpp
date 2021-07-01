#include <stdio.h>
#include "dcore.hpp"
#include "dvec.hpp"

void foo( double& dcore_ref)
{
  printf("value at ref is %f \n",dcore_ref);
  printf("ref addresss is %p \n",&dcore_ref);
}

void bar(double* dcore_ptr)
{
  printf("value at pntr is %f \n",*dcore_ptr);
  printf("pntr address is %p \n",dcore_ptr);
} 

void bar_list(const std::size_t n,double* dcore_ptr)
{
  for (std::size_t i=0;i<n;i++)
  {
    printf("element = %lu , value = %f , adddress = %p \n",
           i,*(dcore_ptr+i),dcore_ptr+i);
  }
} 

int main()
{
  dcore dd;
  dd.info();
  dd.allocate(1000);
  printf("\n");
  dd.info();
  printf("here\n");
  printf("\n");
  printf("\n");

  
  int x=55;
  double* xptr = dd.checkout(x);
  printf("location of x is... %p \n",xptr);
  dd(0) = 10;
  dd(1) = 9;
  dd(2) = 8;
  for (std::size_t i=0;i<4;i++)
  {
    printf("%f ",dd(i));
  }
  printf("\n");
  for (std::size_t i=1;i<4;i++)
  {
    dd(i) -= dd(i)-1;
    printf("%f ",dd(i));
  }
  printf("\n");

  dd(x) = 120;
  printf("size of pointer...%lu \n",sizeof(xptr));
  printf("size of value...%lu \n",sizeof(dd(0))); 
  foo(dd(x));
  bar(&dd(x));
  bar_list(10,&dd(x));

  //Testing dvec memory access
  dvec dv;
  dv.allocate(20);
  dv.info();
  dv.deallocate();
  dv.assign(50,&dd(x));
  dv.info();
  

  return 0;
}



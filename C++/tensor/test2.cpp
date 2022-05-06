#include "tensor.hpp"
int main()
{
  libj::tensor<double> T1(3);
 
  for (size_t I=0;I<T1.size();I++)
  {
    T1(I) = 10+I;
  }

  libj::tensor<double> T2 = T1;
  
  for (size_t I=0;I<T2.size();I++)
  { 
    printf("%lf \n",T2(I));
  }
}

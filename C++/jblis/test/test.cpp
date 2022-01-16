#include "jblis_test.hpp"
#include "jblis.h"
#include "libj.hpp"

int main()
{
  
  //call L1 tests
  pritnf("LEVEL1V TESTS\n")
  dzerov_test1();
  dzerov_test2();
  dzerov_p_test1();
  dzerov_p_test2();
  dsetv_test1();
  dsetv_test2();
  dsetv_p_test1();
  dsetv_p_test2();
  dscalv_test1();
  dscalv_test2();
    

  return 0;
}

#include <stdio.h>
#include <iostream>
#include "dcore.hpp"
#include "dvec.hpp"
#include "dgemat.hpp"
#include "timer.hpp"

int main()
{
  dgemat dm(3,3);
  dm.I();
  dm.print();

  return 0;
}

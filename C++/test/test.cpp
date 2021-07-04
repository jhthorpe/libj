//#include <stdio.h>
//#include <iostream>
//#include "dcore.hpp"
//#include "dvec.hpp"
//#include "dgemat.hpp"
#include "fsys.hpp"
#include <stdio.h>
#include <vector>
#include <string>

int main()
{
  fsys fs;
  fs.print();
  fs.add("foo.bin");
  fs.add("bar.bin");
  long id=fs.fid("bar.bin");
  printf("fid of bar.bin is %ld\n",id);
  fs.print();

  fs.open("foo.bin","w+b");
  fs.print();

  
  return 0;
}

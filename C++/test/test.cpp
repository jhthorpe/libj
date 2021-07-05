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
  fs.add("foo.bin");
  fs.add("bar.bin");
  fs.open("foo.bin","w+b");
  fs.print();

  fs.save();

  FILE* fp;
  fp = fs.get_fptr("foo.bin");
  printf("pointer is... %p \n",fp);
  
  return 0;
}

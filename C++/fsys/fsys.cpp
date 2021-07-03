/*-------------------------------------------------------
  fsys.cpp
	JHT, July 2, 2021
	- implements the functions in fsys.hpp
-------------------------------------------------------*/
#include "fsys.hpp"

/*-------------------------------------------------------
  Initializers and destructors
-------------------------------------------------------*/
fsys::fsys()
{
  fptr.reserve(50);
  fname.reserve(50);
  fmode.reserve(50);
  isopen.reserve(50);
  isfree.reserve(50);
  nfiles=0;
  nfree=fptr.capacity();
  for (std::size_t i=0;i<nfree;i++)
  {
    ptr.push_back(NULL);
    fname[i] = "        ";
    fmode[i] = "   ";
    isopen[i] = false;
    isfree[i] = true;
    isbin[i] = true;
  }
}

fsys::~fsys()
{
  fptr.clear();
  fname.clear();
  fmode.clear();
  isopen.clear();
  isfree.clear();
  nfiles=0;
  nfree=0;
}

/*-------------------------------------------------------
  Adding and removing files 
-------------------------------------------------------*/

std::size_t fsys::add(const char* fn)
{
  //If there are no free files, push new file
  if (nfree = 0) 
  {
    nfiles++;
    FILE* tmp;
    fptr.push_back(tmp); 
    fname.push_back(*fn);
    fmode.push_back("   ");
    isopen.push_back(false);
    isfree.push_back(false); 
  }
  //there are free files, find them
 
}

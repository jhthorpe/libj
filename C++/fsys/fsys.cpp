/*-------------------------------------------------------
  fsys.cpp
	JHT, July 2, 2021
	- implements the functions in fsys.hpp
-------------------------------------------------------*/
#include "fsys.hpp"

//-------------------------------------------------------
//  Initializers and destructors
//-------------------------------------------------------*/
fsys::fsys()
{
  next.reserve(50);
  fptr.reserve(50);
  fname.reserve(50);
//  fmode.reserve(50);
//  isopen.reserve(50);
//  isfree.reserve(50);
  nfiles=0;
  nfree=next.capacity();
}

fsys::~fsys()
{
  //deleting vector of pointers
  int64_t ll = fptr.capacity();
  for (int64_t i=0; i < ll; i++)
  {
    delete fptr[i]; 
  }
}

/*

//---------------------------------------------------------
//  Adding and removing files 
//---------------------------------------------------------
int64_t fsys::add(const std::string fn)
{
  //If there are no free files, push new file
  if (nfree == 0) 
  {
    //defaults, since not specified
    next.push_back(0);
    fptr.push_back(NULL); 
    fname.push_back(fn);
    fmode.push_back("w+b"); 
    isopen.push_back(false);
    isfree.push_back(false); 

    nfiles++;
    return nfiles;
  } else
  //there are free files, find them
  {
    int64_t fid=0;
    while (true)
    {
      if (isfree[fid]) break;
      fid++;
    }
    
    //fid is now file id.  
    next[fid] = 0;
    fptr[fid] = NULL; 
    fname[fid] = fn;
    fmode[fid] = "w+b";
    isopen[fid] = false;
    isfree[fid] = false;
    
    nfiles++;
    nfree--;
   
    return fid;
  }
 
}

//-------------------------------------------------------
//  print() 
//    prints the filesystem
//-------------------------------------------------------
void fsys::print() const
{
  printf("FSYS contains the following: \n");
  printf("%lld files",nfiles);
  printf("%lld of which are free",nfree);

  for (int64_t fid=0;fid<nfiles;fid++)
  {
    if (isfree[fid])
    {
      printf("File ID           : %lld \n",fid);
      printf("File name         : %s \n",fname[fid].c_str());
      printf("File mode         : %s \n",fmode[fid].c_str());
      printf("File is open (t/f):");
      printf("%s", isopen[fid] ? "true \n" : "false \n");
//      printf("File pointer      : %p \n",fptr[fid]);
      printf("Next offset       : %lld \n",next[fid]);
      printf("\n");
    }
  }

  printf("\n");
}

 HERE HERE HERE */

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
  const unsigned long nres=50;
  next.reserve(nres);
  fptr.reserve(nres);
  fname.reserve(nres);
  fmode.reserve(nres);
  isopen.reserve(nres);
  nfiles=0;
  nfree=next.capacity();
}

//We need to delete the pointers by hand, I believe
//smart pointers would be better, but I am a simple
//man
fsys::~fsys()
{
  for (long i=0; i < nfiles; i++)
  {
    delete fptr[i]; 
  }
} 


//---------------------------------------------------------
//  Adding and removing files 
//---------------------------------------------------------
long fsys::add(const std::string fn)
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

    nfiles++;
    return nfiles;
  } else 
  //there are free files, find them
  {
    //fid is now file id.  
    long fid=nfiles;
    next[fid] = 0;
    fptr[fid] = NULL; 
    fname[fid] = fn;
    fmode[fid] = "w+b";
    isopen[fid] = false;
    
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
  printf("%ld files \n",nfiles);

  for (long fid=0;fid<nfiles;fid++)
  {
      printf("\n");
      printf("File ID         : %ld \n",fid);
      printf("File name       : %s \n",fname[fid].c_str());
      printf("File mode       : %s \n",fmode[fid].c_str());
      printf("File is open    : ");
      printf("%s", isopen[fid] ? "true \n" : "false \n");
//      printf("File pointer      : %p \n",fptr[fid]);
      printf("Next offset     : %ld \n",next[fid]);
  }

  printf("\n");
}

//-------------------------------------------------------
// fid(fn)
//	finds the file id (location in vector) of a 
//	particular file name
//
//	The intelligent thing to do would be to retain
//	a sorted list of file names and associated ids,
//	but the list is probably so short that this
//	doesn't matter
//-------------------------------------------------------
long fsys::fid(const std::string fn) const
{
  long id=0;
  while (true)
  {
    if (fn == fname[id]) break;
    id++;
  }
  if (id >= nfiles) 
  {
    printf("fsys could not find file id of file %s",fn.c_str()); 
    exit(1);
  }
  return id; 
}

//-------------------------------------------------------
// open(fn,fm)
//	Opens a file in the filesystem with name fn (string)
//	and mode fm (string)
//-------------------------------------------------------
void fsys::open(const std::string fn, const std::string fm)
{
  long id=fid(fn);

  //Check that file is not already open
  if (!isopen[id])
  {
    fptr[id] = fopen(fn.c_str(),fm.c_str());
    if (fptr[id] != NULL) 
    {
      isopen[id] = true; 
    } else {
      printf("File %s could not be oppened \n",fn.c_str());
      exit(1); 
    }
  }
}

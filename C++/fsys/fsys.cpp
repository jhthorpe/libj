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

fsys::~fsys()
{
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
    fmode.push_back("r+b"); 
    isopen.push_back(false);

    nfiles++;
    return nfiles;
  } else 
  //there are free files, find them
  {
    //fid is now file id.  
    long id=nfiles;
    next[id] = 0;
    fptr[id] = NULL; 
    fname[id] = fn;
    fmode[id] = "r+b";
    isopen[id] = false;
    
    nfiles++;
    nfree--;
   
    return id;
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

  for (long id=0;id<nfiles;id++)
  {
      printf("\n");
      printf("File ID         : %ld \n",id);
      printf("File name       : %s \n",fname[id].c_str());
      printf("File mode       : %s \n",fmode[id].c_str());
      printf("File is open    : ");
      printf("%s", isopen[id] ? "true \n" : "false \n");
      printf("File pointer      : %p \n",fptr[id]);
      printf("Next offset     : %ld \n",next[id]);
  }

  printf("\n");
}

//-------------------------------------------------------
// get_fid(fn)
//	finds the file id (location in vector) of a 
//	particular file name
//
//	The intelligent thing to do would be to retain
//	a sorted list of file names and associated ids,
//	but the list is probably so short that this
//	doesn't matter
//-------------------------------------------------------
long fsys::get_fid(const std::string fn) const
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
// get_ptnr(fn)
//	returns the file pointer associated with a 
//	particular file name
//-------------------------------------------------------
FILE* fsys::get_fptr(const std::string fn) const
{
  const long fid=get_fid(fn);
  return fptr[fid];
}

//-------------------------------------------------------
// set_mode(fn,fm)
//	sets the mode of a particular file with name
//	fn (string) and mode fm (string) 
//-------------------------------------------------------
void fsys::set_mode(const std::string fn, const std::string fm)
{
  const long fid=get_fid(fn);
  fmode[fid] = fm;
}

//-------------------------------------------------------
// open(fn,fm)
//	Opens a file in the filesystem with name fn (string)
//	and mode fm (string)
//-------------------------------------------------------
void fsys::open(const std::string fn, const std::string fm)
{
  long id=get_fid(fn);

  //Check that file is not already open
  if (!isopen[id])
  {
    fptr[id] = fopen(fn.c_str(),fm.c_str());
    if (fptr[id] != NULL) 
    {
      isopen[id] = true; 
      fmode[id] = fm;
    } else {
      printf("File %s could not be oppened \n",fn.c_str());
      exit(1); 
    }
  }
}

//-------------------------------------------------------
// close(fn)
//	Closes a file in the filesystem with name 
//      fn (string) and mode fm (string)
//-------------------------------------------------------
void fsys::close(const std::string fn)
{
  long id=get_fid(fn);

  //Check that file open 
  if (isopen[id])
  {
    fclose(fptr[id]);
    isopen[id] = false;
    fptr[id] = NULL;
  }
}

//-------------------------------------------------------
// close_all()
//	Closes all files in the fileystem
//-------------------------------------------------------
void fsys::close_all()
{
  for (long fid=0;fid<nfiles;fid++)
  {
    if (isopen[fid]) 
    {
      fclose(fptr[fid]);
      isopen[fid] = false;
      fptr[fid] = NULL;
    }
  }
}

//-------------------------------------------------------
// open_all()
//	Opens all files in the fileystem
//-------------------------------------------------------
void fsys::open_all()
{
  for (long fid=0;fid<nfiles;fid++)
  {
    if (!isopen[fid])
    {
      fptr[fid] = fopen(fname[fid].c_str(),fmode[fid].c_str());
      if (fptr[fid] != NULL) 
      {
        isopen[fid] = true;
      } else {
        printf("Could not open file %s \n",fname[fid].c_str());
        exit(1);
      }
    }
  }
}

//-------------------------------------------------------
// save()
//	save the filesystem data to fsys
//
//	NOTE : we don't save the file pointers, this is
//	       probably incorrect anyways
//-------------------------------------------------------
void fsys::save() const
{
  FILE* fp;
  fp = fopen("fsys.save","w+b"); //"w" will delete old file for us
  if (fp != NULL) 
  {
    bool btmp; //annoyingly needed for vector with bools...
    fwrite(&nfiles,sizeof(nfiles),1,fp); 
    for (long fid=0;fid<nfiles;fid++)
    {
      btmp = isopen[fid];
      fwrite(&next[fid],sizeof(next[fid]),1,fp);
      fwrite(&fname[fid],sizeof(fname[fid]),1,fp);
      fwrite(&fmode[fid],sizeof(fmode[fid]),1,fp);
      fwrite(&btmp,sizeof(bool),1,fp);
    }
  } else {
    printf("Could not open fsys.save \n");
    exit(1);
  }
  fclose(fp);
}

//-------------------------------------------------------
// recover()
//	recovers the filesystem
//
//	This will reopen all files marked as open
//-------------------------------------------------------
void fsys::recover()
{
  FILE* fp;
  fp = fopen("fsys.save","rb");
  if (fp != NULL) 
  {
    //get number of files
    fread(&nfiles,sizeof(nfiles),1,fp);
   
    //reserve space if it isn't enough
    const long cap=next.capacity();
    const long nt2=2*nfiles;
    if (cap < nt2 ) 
    {
      next.reserve(nt2);
      fptr.reserve(nt2);
      fname.reserve(nt2);
      fmode.reserve(nt2);
      isopen.reserve(nt2);
    }
    //allocate needed memory
    next.resize(nfiles);
    fptr.resize(nfiles);
    fname.resize(nfiles);
    fmode.resize(nfiles);
    isopen.reserve(nfiles);
    
    //temp variables for vector read/write
    long                 ltmp;
    std::string   stmp1,stmp2;
    bool                 btmp;
    for (long fid=0;fid<nfiles;fid++)
    {
      fread(&ltmp,sizeof(ltmp),1,fp);
      fread(&stmp1,sizeof(stmp1),1,fp);
      fread(&stmp2,sizeof(stmp2),1,fp);
      fread(&btmp,sizeof(btmp),1,fp);

      next[fid] = ltmp;
      fname[fid] = stmp1;
      fmode[fid] = stmp2;
      isopen[fid] = btmp;
    }

    //go through and open files that were marked as open
    for (long fid=0;fid<nfiles;fid++)
    {
      if (!isopen[fid]) continue; 

      fptr[fid] = fopen(fname[fid].c_str(),fmode[fid].c_str());
      if (fptr[fid] == NULL) 
      {
        printf("Could not open file %s during recovery \n",fname[fid].c_str());
        exit(1);
      }
    }
    
  //Could not open the fsys.save file 
  } else {
    printf("Could not recover filesystem from fsys.save \n");
    exit(1);
  }
  fclose(fp);
}

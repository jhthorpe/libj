/*--------------------------------------------------------------------------- 
  para.hpp
	JHT, Febuary 21, 2022 : created

  .cpp file for the para class object, which is the interaface to the other
  para classes and routines
---------------------------------------------------------------------------*/
#include "para.hpp"

//---------------------------------------------------------------------------
// Para() -- initialization
//---------------------------------------------------------------------------
int Para::init()
{
  if (pworld.init() != 0) {error(-1);}
  if (pprint.init(pworld) != 0) {error(-1);}
  if (pworld.mpi_doesIO) {
    if (pfile.init(pworld) != 0) {error(-1);}
  }
  return 0;
}

//---------------------------------------------------------------------------
// Para() -- destruction
//---------------------------------------------------------------------------
int Para::destroy()
{
  if (pprint.destroy(pworld) != 0) {error(1);}
  if (pworld.destroy() != 0) {error(1);}
  return 0;
}

//---------------------------------------------------------------------------
// Error
//---------------------------------------------------------------------------
void Para::error(const int stat)
{
  if (pworld.mpi_world_ismaster) {printf("para error status %d\n",stat);}
  #if defined LIBJ_MPI
    MPI_Barrier(pworld.comm_world);
  #endif
  destroy();
  exit(stat);
}

//---------------------------------------------------------------------------
// print_add
//---------------------------------------------------------------------------
int Para::print_add(const char* fstring,...)
{
  va_list arg;
  va_start(arg,fstring);
  int stat = pprint.vadd(fstring,arg); 
  va_end(arg);
  return stat;
}

//---------------------------------------------------------------------------
// print_store
//---------------------------------------------------------------------------
int Para::print_store()
{
  pprint.store();
  return 0;
}

//---------------------------------------------------------------------------
// print_addstore
//---------------------------------------------------------------------------
int Para::print_addstore(const char* fstring,...)
{
  va_list arg;
  va_start(arg,fstring);
  int stat = pprint.vadd(fstring,arg); 
  va_end(arg);
  pprint.store();
  return stat;
}

//---------------------------------------------------------------------------
// print_now
//---------------------------------------------------------------------------
int Para::print_now(const char* fstring,...)
{
  va_list arg;
  va_start(arg,fstring);
  int stat = pprint.vadd(fstring,arg); 
  va_end(arg);
  pprint.store();
  pprint.print_all(pworld);
  pprint.reset();
  return stat;
}

//---------------------------------------------------------------------------
// print_reset
//---------------------------------------------------------------------------
int Para::print_reset()
{
  pprint.reset();
  return 0;
}

//---------------------------------------------------------------------------
// print_all_noreset
//---------------------------------------------------------------------------
int Para::print_all_noreset()
{
  pprint.print_all(pworld);
  return 0;
}
//---------------------------------------------------------------------------
// print_all
//---------------------------------------------------------------------------
int Para::print_all()
{
  pprint.print_all(pworld);
  pprint.reset();
  return 0;
}

//---------------------------------------------------------------------------
// print_master_add
//---------------------------------------------------------------------------
int Para::print_master_add(const char* fstring,...)
{
  int stat = 0;
  if (pworld.mpi_world_ismaster)
  {
    va_list arg;
    va_start(arg,fstring);
    stat = pprint.vadd(fstring,arg); 
    va_end(arg); 
  }
  return stat;
}

//---------------------------------------------------------------------------
// print_master_addstore
//---------------------------------------------------------------------------
int Para::print_master_addstore(const char* fstring,...)
{
  int stat = 0;
  if (pworld.mpi_world_ismaster)
  {
    va_list arg;
    va_start(arg,fstring);
    stat = pprint.vadd(fstring,arg); 
    va_end(arg); 
  }
  pprint.store(); //this keeps the message count synched up
  return stat;
}

//---------------------------------------------------------------------------
// print_master_now
//---------------------------------------------------------------------------
int Para::print_master_now(const char* fstring,...)
{
  int stat = 0;
  if (pworld.mpi_world_ismaster)
  {
    va_list arg;
    va_start(arg,fstring);
    stat = pprint.vadd(fstring,arg); 
    va_end(arg); 
  }
  pprint.store(); //this keeps the message count synched up
  pprint.print_all(pworld);
  pprint.reset();
  return stat;
}

//---------------------------------------------------------------------------
// file_add
//	add a file to the filesystem, synchronise across tasks on shared
//	memory
//---------------------------------------------------------------------------
int Para::file_add(const char* fname)
{
  int file_id = -1;
  //add the file
  if (pworld.mpi_doesIO) {file_id = pfile.sadd(pworld,fname);}

  //Bcast 
  #ifdef LIBJ_MPI
  MPI_Barrier(pworld.comm_shared);
  MPI_Bcast(&file_id,1,MPI_INT,pworld.mpi_shared_root,pworld.comm_shared);
  #endif

  //return
  return file_id; 
}


//---------------------------------------------------------------------------
// file_open
//	opens file associated with file_id, with args 
//---------------------------------------------------------------------------
int Para::file_open(const int file_id, const char* arg)
{
  int stat;
  if (pworld.mpi_doesIO) {stat = pfile.xopen(file_id,arg);} 

  #ifdef LIBJ_MPI
  MPI_Barrier(pworld.comm_shared);
  MPI_Bcast(&stat,1,MPI_INT,pworld.mpi_shared_root,pworld.comm_shared);
  #endif

  return stat;
}

//---------------------------------------------------------------------------
// file_close
//	closes the file associated with file_id
//---------------------------------------------------------------------------
int Para::file_close(const int file_id)
{
  int stat;
  if (pworld.mpi_doesIO) {stat = pfile.xclose(file_id);} 

  #ifdef LIBJ_MPI
  MPI_Barrier(pworld.comm_shared);
  MPI_Bcast(&stat,1,MPI_INT,pworld.mpi_shared_root,pworld.comm_shared);
  #endif
  
  return stat;
}

//---------------------------------------------------------------------------
// file_isopen
//	returns true if file is open
//---------------------------------------------------------------------------
bool Para::file_isopen(const int file_id)
{
  bool tf;
  int stat;
  if (pworld.mpi_doesIO) {tf = pfile.xisopen(file_id);}
  stat = (tf) ? 0 : 1;

  #ifdef LIBJ_MPI
  MPI_Barrier(pworld.comm_shared);
  MPI_Bcast(&stat,1,MPI_INT,pworld.mpi_shared_root,pworld.comm_shared);
  #endif

  return (stat == 0) ? true : false;
}

//---------------------------------------------------------------------------
// file_print_info()
//	puts file info into the print buffer
//---------------------------------------------------------------------------
void Para::file_print_info()
{
  pfile.info(pworld,pprint);
}

//---------------------------------------------------------------------------
// file_save
//	saves the filesystem to disk 
//---------------------------------------------------------------------------
int Para::file_save()
{
  return pfile.save(pworld);
}

//---------------------------------------------------------------------------
// file_recover
//	recovers the filesystem from disk 
//---------------------------------------------------------------------------
int Para::file_recover()
{
  return pfile.recover(pworld);
}

//---------------------------------------------------------------------------
// file_getid
//---------------------------------------------------------------------------
int Para::file_getid(const char* fname)
{
  int fid = -1;
  if (pworld.mpi_doesIO) {fid = pfile.sget_fid(pworld,fname);}

  #ifdef LIBJ_MPI
  MPI_Barrier(pworld.comm_shared);
  MPI_Bcast(&fid,1,MPI_INT,pworld.mpi_shared_root,pworld.comm_shared);
  #endif

  return fid;
}

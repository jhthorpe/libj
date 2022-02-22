/*--------------------------------------------------------------------
  para.hpp
	JHT, Febuary 21, 2022 : created

  .hpp for the para class, which is the interface to the other para
  classes and routines.

  The intention of this class is to make task-based 
  parallelization as "OMP-like" as possible. 

  Access to specific instances of lists or files is provided by a 
  "file_id" or "list_id". 

  ----------------------------------
  INIT, DESTROY, and ERROR
    - init and destroy *must* be called at the begining and end of the 
      parallel section
    - error can be called to terminate the program
  Para para;
  para.init();
  para.destroy();
  para.error(1);

  ----------------------------------
  PRINTING TO STDIO 
    -  messages are "added" by concatination to a string buffer. When a 
       message is finished, it is "stored". Then, all messages from all
       tasks are printed in the stored order (and comm_world order). 
    - "master" in the function name indicates that only the master task 
       actually stores a non-blank message
    -  "now" in the function name indicates that this message will be 
       printed right away, along with all other messages in the queue 

    Usage example:
    para.print_add("hello");
    para.print_addstore(" world! 42 = %d \n",42);
    para.print_all();
    para.print_master_now("master prints this right away\n");
  

  ----------------------------------
  FILESYSTEM
    - note that the user should manage the file_ids on their
      own, either via a dictionary or by initializing in the same order
      each time. These can be recovered via the file_getid function, if
      needed
    - fileIO is performed by the shared_root task for each set of tasks on
      a shared memory machine. 
   
   Usage example:
   const int fid = para.file_add("data");
   para.file_open(fid,"w+b");
  
   para.file_close();
     
  
  ----------------------------------
  DATASYSTEM 

--------------------------------------------------------------------*/
#ifndef LIBJ_PARA_HPP
#define LIBJ_PARA_HPP

#include "pworld.hpp"
#include "strvec.hpp"
#include "pprint.hpp"
#include "pfile.hpp"
#include "pdata.hpp"

class Para
{

  public:
  Pworld pworld;
  Pprint pprint;
  Pfile  pfile;
  Pdata  pdata;

  //init, destory, and error functions
  int init();
  int destroy();
  void error(const int stat);

  //PRINTING
  // add, store, or reset the printing queue
  int print_add(const char* fstring,...); 
  int print_master_add(const char* fstring,...);
  int print_store();
  int print_addstore(const char* fstring,...);
  int print_master_addstore(const char* fstring,...);
  int print_reset();
  int print_all();
  int print_all_noreset();
  int print_now(const char* fstring,...);
  int print_master_now(const char* fstring,...);

  //FILESYSTEM
  int file_add(const char* fname);
  int file_getid(const char* fname);
  int file_open(const int file_id, const char* arg);
  int file_close(const int file_id);
  bool file_isopen(const int file_id);
  void file_print_info();
  int file_save();
  int file_recover();

  
};

#endif

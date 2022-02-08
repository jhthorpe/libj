/*-----------------------------------------------------------------
  pworld.hpp
	JHT, Febuary 7, 2022 : created

  .hpp file for Pworld, which manages the initialization and 
  finalization of MPI parameters if they are required

//Usage
init()		: initializes variables and structures
destroy()	: finalizes variables and structures
info()		: prints info about the world

//Communicators
comm_world 	: MPI_COMM_WORLD

-----------------------------------------------------------------*/
#ifndef PWORLD_HPP
#define PWORLD_HPP
#include <stdio.h>

#include "libjdef.h"

#if defined LIBJ_MPI
  #include <mpi.h>
#endif

#if defined LIBJ_OMP
  #include <omp.h>
#endif

struct Pworld
{
  public:
  
  //Basic Data
  int mpi_num_tasks;
  int mpi_task_id;
  int omp_num_threads;
  bool ismpi;
  bool isomp;
  bool mpi_ismaster;

  //MPI communicators
  #if defined LIBJ_MPI
    MPI_Comm comm_world = MPI_COMM_WORLD;
    //Other communicators?
  #endif

  //Initializer
  int init()
  {
    #if defined LIBJ_MPI
      MPI_Init(NULL,NULL);
      MPI_Comm_size(comm_world,&mpi_num_tasks);
      MPI_Comm_rank(comm_world,&mpi_task_id);
      if (mpi_task_id == 0) {mpi_ismaster = true;}
      ismpi = true;
    #else
      mpi_num_tasks=1;
      mpi_task_id=0;
      mpi_ismaster=true;
      ismpi = false;
    #endif

    #if defined LIBJ_OMP
      #pragma omp parallel shared(omp_num_threads) 
      {
        #pragma omp single
        {
          omp_num_threads = omp_get_num_threads(); 
        }
      }
      isomp = true;
    #else
      omp_num_threads=1;
      isomp = false;
    #endif
    return 0;
  }
  
  //Destruction
  int destroy() 
  {
    #if defined LIBJ_MPI
      MPI_Finalize();
    #endif
    return 0;
  }

};

#endif
/*------------------------------------------------------------------------
  pprint.h
	JHT, Feburary 4, 2022 : created

  .h file for Prprint and Stringvec

  This allows for gennerating buffered print statements, which should help 
  synchronize prints across tasks

  NOTE : init and destory *must* be called at the start and end  

//INITIALIZE AND DESTROY
buf.init(const Pworld& pworld);
buf.destroy(const Pworld& pworld);

// To add to the current buffer
buf.add(" %d %lf.18  ",10,18.5); //add formatted string to buffer

//To store the corrent buffer as a message
buf.store("tea time");		 //store the buffer as a whole message

//To add and store to the buffer as a message
buf.addstore("testing testing"); //add and store buffer in one step

//Printing
buf.print_all();		 //print all messages
buf.print(2);                    //prints the n'th message, index from zero

//Clearing
buf.clear();			 //clears an unstored buffer
buf.reset();			 //reset buffer and stored messages

-------------------------------------------------------------------------*/
#ifndef PPRINT_HPP
#define PPRINT_HPP
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libjdef.h"

#define PPRINT_RES 10
#define PPRINT_LEN 1024 

#if defined LIBJ_MPI
  #include <mpi.h> 
#endif

#if defined LIBJ_OMP
  #include <omp.h>
#endif

/* 
 *  Stringvec
 *  a lightweight implementation of std::vector for C-style strings
*/
struct Stringvec
{
  //data
  int   size; 		//number of elements
  int   capacity; 	//number of reserved elements
  char* buffer; 	//buffer
  
  //Initializer
  Stringvec() 
  {
    buffer = (char*) malloc(sizeof(char)*PPRINT_LEN*PPRINT_RES); 
    if (buffer != NULL)
    {
      size = 0;
      capacity = PPRINT_RES;
    } else {
      printf("\nERROR ERROR ERROR\n");
      printf("Stringvec::init could not malloc buffer\n");
      exit(1);
    }
  }

  //destructor
  ~Stringvec() 
  {
    if (buffer != NULL) {free(buffer);}
  }

  //accessing vector elements
  char* operator[] (const long elem) {return (buffer+PPRINT_LEN*elem);}
  const char* operator[] (const long elem) const {return (buffer+PPRINT_LEN*elem);} 

  //grow
  void grow() 
  {
    char* newbuf = (char*) malloc(sizeof(char)*capacity*2*PPRINT_LEN);
    memmove(newbuf,buffer,sizeof(char)*capacity*PPRINT_LEN);
    memset(newbuf+size*PPRINT_LEN,(char)0,sizeof(char)*capacity*PPRINT_LEN);
    capacity *= 2;
    free(buffer);
    buffer = newbuf;
  }

  //push_back
  void push_back(const char* string)
  {
    if (size+1 > capacity) {grow();}
    strcpy(buffer+size*PPRINT_LEN,string);
    size++;
  }

  //clear
  void clear()
  {
    memset(buffer,(char)0,sizeof(char)*size*PPRINT_LEN); 
    size = 0;
  }

  //info
//  void info() const
//  {
//    printf("Stringvec has size %d and capacity %d\n",size,capacity);
//  }

  //free the memory
  void destroy() 
  {
    free(buffer);
  }
  
};

/*
 * Pprint
 * Buffer for asynchronous printing
*/
struct Pprint
{
  int       idx;
  char      stemp[PPRINT_LEN];
  char      buffer[PPRINT_LEN];
  char*     pbuffer;
  Stringvec vec;

  //Initializer
  Pprint()
  {
    idx = 0;
    memset(buffer,(char)0,sizeof(char)*PPRINT_LEN);
    memset(stemp,(char)0,sizeof(char)*PPRINT_LEN);
  }

  ~Pprint()
  {
        
  }

  //Initialize with Pworld
  int init(const Pworld& pworld)
  {
    int stat = 0;
    if (pworld.mpi_ismaster)
    {
      pbuffer = (char*) malloc(sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
      if (pbuffer != NULL)
      {
        stat = 0;
      } else {
        printf("\nERROR ERROR ERORR\n");
        printf("Pprint::init could not malloc %ld bytes\n",
               sizeof(char)*PPRINT_LEN*pworld.mpi_num_tasks);
        stat = 1;
      }
    }

    return stat;
  }

  //Destructor
  int destroy(const Pworld& pworld) 
  {
    int stat = 0;
    if (pworld.mpi_ismaster)
    {
      free(pbuffer);
    }
    return stat;
  }

  //Add a formatted string with variable input data
  int add(const char* fstring,...)
  {
    int stat=0;

    //if buffer has no elements 
    if (idx == 0)
    {
      va_list arg;
      va_start(arg,fstring); 
      stat = vsprintf(buffer,fstring,arg); 
      va_end(arg);

      idx = strlen(buffer);

    //buffer has elements, we need to concatinate
    } else {
      va_list arg;
      va_start(arg,fstring); 
      stat = vsprintf(stemp,fstring,arg); 
      va_end(arg);
      
      int len = strlen(stemp);
      if (idx + len < PPRINT_LEN)
      {
        memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
        memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
        idx += len;
      } else {
        printf("\nERROR ERROR ERROR\n");
        printf("Prprint::add string would case buffer overflow\n");
        return 1;
      }
      
    }

    return stat;
  }


  //Add a formatted string with variable input data
  int vadd(const char* fstring,va_list arg)
  {
    int stat=0;

    //if buffer has no elements 
    if (idx == 0)
    {
      stat = vsprintf(buffer,fstring,arg); 

      idx = strlen(buffer);

    //buffer has elements, we need to concatinate
    } else {
      stat = vsprintf(stemp,fstring,arg); 
      
      int len = strlen(stemp);
      if (idx + len < PPRINT_LEN)
      {
        memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
        memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
        idx += len;
      } else {
        printf("\nERROR ERROR ERROR\n");
        printf("Prprint::add string would case buffer overflow\n");
        return 1;
      }
      
    }

    return stat;
  }

  //Store buffer into vec, clear buffer
  void store()
  {
    if (idx > 0)
    {
      vec.push_back(buffer);
      clear();
    }
  }

  //Add and store
  void addstore(const char* fstring, ...)
  {
    va_list arg;
    va_start(arg,fstring);
    vadd(fstring,arg);
    va_end(arg);
    store();
  }

  //clear buffer
  void clear()
  {
    memset(buffer,(char)0,sizeof(char)*idx);
    idx = 0;
  }

  //reset whole thing
  void reset()
  {
    vec.clear();
    clear();
  }

  //print all messages
  void print_all(const Pworld& pworld) const;

  //print specific messages
  void print(const Pworld& pworld, const int message) const;

  //print info
//  void info() const
//  {
//    printf("\nPprint has %d entries\n",vec.size);
//    printf("Entry max size is %d\n",PPRINT_LEN);
//  }

  //print specific message
//  void print(const int message) const
//  {
//    printf("%s",vec[message]);
//  }

  int size() const {return vec.size;}

};

#endif
/* 
 *  Strvec
 *  a lightweight implementation of std::vector for C-style strings
*/
#ifndef STRVEC_HPP
#define STRVEC_HPP

#include "libjdef.h"
#include <stdio.h>
#include <string.h>

class Strvec
{
  //data
  private:
  int   m_size;       //number of elements
  int   m_capacity;   //number of reserved elements
  int   m_STR_LEN;    //length of char[]
  char* m_buffer;     //m_buffer

  public: 

  //accessing vector elements
  char* operator[] (const long elem) {return (m_buffer+m_STR_LEN*elem);}
  const char* operator[] (const long elem) const {return (m_buffer+m_STR_LEN*elem);}

  int size() const {return m_size;}
  int capacity() const {return m_capacity;}
  int str_len() const {return m_STR_LEN;}
  
  //Initializer
  Strvec(const int STR_LEN)
  {
    m_STR_LEN = STR_LEN;
    m_buffer = (char*) malloc(sizeof(char)*m_STR_LEN);
    if (m_buffer != NULL)
    {
      m_size = 0;
      m_capacity = PPRINT_RES;
    } else {
      printf("\nERROR ERROR ERROR\n");
      printf("Strvec::init could not malloc m_buffer\n");
      exit(1);
    }
  }

  //destructor
  ~Strvec()
  {
    if (m_buffer != NULL) {free(m_buffer);}
  }
  
  //grow
  void grow()
  {
    char* newbuf = (char*) malloc(sizeof(char)*m_capacity*2*m_STR_LEN);
    memmove(newbuf,m_buffer,sizeof(char)*m_capacity*m_STR_LEN);
    memset(newbuf+m_size*m_STR_LEN,(char)0,sizeof(char)*m_capacity*m_STR_LEN);
    m_capacity *= 2;
    free(m_buffer);
    m_buffer = newbuf;
  }

  //push_back
  void push_back(const char* string)
  {
    if (m_size+1 > m_capacity) {grow();}
    strcpy(m_buffer+m_size*m_STR_LEN,string);
    m_size++;
  }

  //clear
  void clear()
  {
    memset(m_buffer,(char)0,sizeof(char)*m_size*m_STR_LEN);
    m_size = 0;
  }

  //info
//  void info() const
//  {
//    printf("Strvec has m_size %d and m_capacity %d\n",m_size,m_capacity);
//  }

  //free the memory
  void destroy()
  {
    free(m_buffer);
  }

};

#endif
/*------------------------------------------------------------------------
 * pfile.hpp
 *  JHT, Febuary 8, 2022: created
 *
 *  .hpp file for Pfile, which handles a (possibly parallel) filesystem
 *
------------------------------------------------------------------------*/
#ifndef PFILE_HPP
#define PFILE_HPP

#include <string>
#include <vector>
#include <stdio.h>
#include "libjdef.h"

#define PFILE_RES 50

class Pfile
{
  private: 
  //Data
  std::vector<bool>        m_isopen;  //bools for tracking if file is open
  std::vector<bool>        m_isbin;   //bools if file is binary 
  std::vector<FILE*>       m_fptr;    //file pointers
  std::vector<long>        m_fpos;    //current file positions (bytes)
  std::vector<std::string> m_fname;   //file names
  std::vector<std::string> m_fstat;   //file status
  int                      m_nfiles;  //number of files

  public:
  //Initializer
  Pfile();
  //Destructor
  ~Pfile();

  //Add a file
  //Remove a file
  
  //info
  void info(const Pworld& pworld, Pprint& pprint) const;

  //recover
  
  //Open all files
  //Close all files
  //Open a file
  //Close a file
  //check if file is open and with what status

};


#endif

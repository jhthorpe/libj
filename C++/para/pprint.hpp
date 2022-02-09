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
      memset(buffer,(char)0,sizeof(char)*capacity*PPRINT_LEN);
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
    pbuffer = NULL;
  }

  //Destructor
  ~Pprint()
  {
    if (pbuffer != NULL) free(pbuffer);        
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
      if (pbuffer != NULL) free(pbuffer);
      pbuffer = NULL;
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

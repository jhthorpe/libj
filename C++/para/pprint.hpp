/*------------------------------------------------------------------------
  pprint.h
	JHT, Feburary 4, 2022 : created

  .h file for Prprint and Stringvec

  This allows for gennerating buffered print statements, which should help 
  synchronize prints across tasks

  NOTE : init and destory *must* be called at the start and end  

  NOTE : pprint works specificially with the WORLD communicator to gather
	 all the output

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
#include "pworld.hpp"
#include "strvec.hpp"

#define PPRINT_RES 10
#define PPRINT_LEN 1024 

#if defined LIBJ_MPI
  #include <mpi.h> 
#endif

#if defined LIBJ_OMP
  #include <omp.h>
#endif


/*
 * Pprint
 * Buffer for asynchronous printing
*/
struct Pprint
{
  int                idx;
  char               stemp[PPRINT_LEN];
  char               buffer[PPRINT_LEN];
  char*              pbuffer;
  Strvec<PPRINT_LEN> vec;

  //Initializer
  Pprint();

  //Destructor
  ~Pprint();

  //Initialize with Pworld
  int init(const Pworld& pworld);

  //Destructor
  int destroy(const Pworld& pworld);

  //Add a formatted string with variable input data
  int add(const char* fstring,...);

  //Add a formatted string with variable input data
  int vadd(const char* fstring,va_list arg);

  //Store buffer into vec, clear buffer
  void store();

  //Add and store
  void addstore(const char* fstring, ...);

  //clear buffer
  void clear();

  //reset whole thing
  void reset();

  //print all messages
  void print_all(const Pworld& pworld) const;

  //print specific messages
  void print(const Pworld& pworld, const int message) const;

  //get size
  int size() const {return vec.size;}

};

#endif

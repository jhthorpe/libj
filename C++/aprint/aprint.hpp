/*------------------------------------------------------------------------
  aprint.hpp 
	JHT, Feburary 4, 2022 : created

  .hpp file for Printbuffer and Stringvec

  This allows for gennerating buffered print statements, which should help 
  synchronize prints across tasks


//USAGE
buf.add(" %d %lf.18  ",10,18.5); //add formatted string to buffer
buf.store("tea time");		 //store the buffer as a whole message
buf.addstore("testing testing"); //add and store buffer in one step
buf.print_all();		 //print all messages
buf.print(2);                    //prints the n'th message, index from zero
buf.clear();			 //clears an unstored buffer
buf.reset();			 //reset buffer and stored messages

-------------------------------------------------------------------------*/
#include <stdio.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "libjdef.h"

#define APRINT_RES 10
#define APRINT_LEN 1024 

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
    buffer = (char*) malloc(sizeof(char)*APRINT_LEN*APRINT_RES); 
    size = 0;
    capacity = APRINT_RES;
  }

  //destructor
  ~Stringvec()
  {
    free(buffer);
  }

  //accessing vector elements
  char* operator[] (const long elem) {return (buffer+APRINT_LEN*elem);}
  const char* operator[] (const long elem) const {return (buffer+APRINT_LEN*elem);} 

  //grow
  void grow() 
  {
    char* newbuf = (char*) malloc(sizeof(char)*capacity*2*APRINT_LEN);
    std::memmove(newbuf,buffer,sizeof(char)*capacity*APRINT_LEN);
    std::memset(newbuf+size*APRINT_LEN,(char)0,sizeof(char)*capacity*APRINT_LEN);
    capacity *= 2;
    free(buffer);
    buffer = newbuf;
  }

  //push_back
  void push_back(const char* string)
  {
    if (size+1 > capacity) {grow();}
    std::strcpy(buffer+size*APRINT_LEN,string);
    size++;
  }

  //clear
  void clear()
  {
    std::memset(buffer,(char)0,sizeof(char)*size*APRINT_LEN); 
    size = 0;
  }

  //info
  void info() const
  {
    printf("Stringvec has size %d and capacity %d\n",size,capacity);
  }

  //free the memory
  void release() 
  {
    free(buffer);
  }
  
};

/*
 * Printbuffer
 * Buffer for asynchronous printing
*/
struct Printbuffer
{
  int       idx;
  char      stemp[APRINT_LEN];
  char      buffer[APRINT_LEN];
  Stringvec vec;

  //Initializer
  Printbuffer()
  {
    idx = 0;
    std::memset(buffer,(char)0,sizeof(char)*APRINT_LEN);
    std::memset(stemp,(char)0,sizeof(char)*APRINT_LEN);
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
      if (idx + len < APRINT_LEN)
      {
        std::memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
        std::memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
        idx += len;
      } else {
        printf("\nERROR ERROR ERROR\n");
        printf("Printbuffer::add string would case buffer overflow\n");
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
      if (idx + len < APRINT_LEN)
      {
        std::memmove(&buffer[idx],stemp,sizeof(char)*len);//move stemp elements
        std::memset(stemp,(char)0,sizeof(char)*len);//clear stemp elements
        idx += len;
      } else {
        printf("\nERROR ERROR ERROR\n");
        printf("Printbuffer::add string would case buffer overflow\n");
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

  //add and store, just a string
  void addstore(const char* string)
  {
    add(string);
    store();
  }

  //Add and store, but formatted
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
    std::memset(buffer,(char)0,sizeof(char)*idx);
    idx = 0;
  }

  //reset whole thing
  void reset()
  {
    vec.clear();
    clear();
  }

  //print buffer
  void print_all() const
  {
    for (int i=0;i<vec.size;i++)
    {
      printf("%s",vec[i]);
    }
  }

  void info() const
  {
    printf("\nPrintbuffer has %d entries\n",vec.size);
    printf("Entry max size is %d\n",APRINT_LEN);
  }

  void print(const int message) const
  {
    printf("%s",vec[message]);
  }

  int size() const {return vec.size;}

};

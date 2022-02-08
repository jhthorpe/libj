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

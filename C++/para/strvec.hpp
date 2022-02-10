/*-------------------------------------------------------------
  Strvec.hpp
	JHT, Febuary 10, 2022 : created

  .hpp file for Strvec, a C++ vector-style impementation of 
  C-like char arrays

  Note that a different class is created for each of the 
  string lengths needed by para. To add a new string length,
  it must be added to the template declarations at the end of
  this .hpp file and the end of the relevant .cpp file 
-------------------------------------------------------------*/
#ifndef STRVEC_HPP
#define STRVEC_HPP
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

template<const int STRLEN>
struct Strvec
{
  //data
  int           size;           //number of elements
  int           capacity;       //number of reserved elements
  char*         buffer;         //buffer

  //Initializer
  Strvec();

  //destructor
  ~Strvec();

  //accessing vector elements
  char* operator[] (const long elem) {return (buffer+STRLEN*elem);}
  const char* operator[] (const long elem) const {return (buffer+STRLEN*elem);}

  //grow
  void grow();

  //push_back
  void push_back(const char* string);

  //clear
  void clear();

  //free the memory
  void destroy();

  int maxlen() const {return STRLEN;}

};

//--------------------------------------------------------
// Strvec initialize
//--------------------------------------------------------
template<const int STRLEN>
Strvec<STRLEN>::Strvec() 
{
  buffer = (char*) malloc(sizeof(char)*STRLEN); 
  if (buffer != NULL)
  {
    size = 0;
    capacity = 1; 
    memset(buffer,(char)0,sizeof(char)*capacity*STRLEN);
  } else {
    printf("\nERROR ERROR ERROR\n");
    printf("Strvec<STRLEN>::init could not malloc buffer\n");
    exit(1);
  }
}

//--------------------------------------------------------
// Strvec destructor
//--------------------------------------------------------
template<const int STRLEN>
Strvec<STRLEN>::~Strvec()
{
  if (buffer != NULL) {free(buffer);}
}

//--------------------------------------------------------
// Grow 
//--------------------------------------------------------
template<const int STRLEN>
void Strvec<STRLEN>::grow()
{
  char* newbuf = (char*) malloc(sizeof(char)*capacity*2*STRLEN);
  memmove(newbuf,buffer,sizeof(char)*capacity*STRLEN);
  memset(newbuf+size*STRLEN,(char)0,sizeof(char)*capacity*STRLEN);
  capacity *= 2;
  free(buffer);
  buffer = newbuf;
}

//--------------------------------------------------------
// push_back
//--------------------------------------------------------
template<const int STRLEN>
void Strvec<STRLEN>::push_back(const char* string)
{
  if (size+1 > capacity) {grow();}
  strcpy(buffer+size*STRLEN,string);
  size++;
}

//--------------------------------------------------------
// clear 
//--------------------------------------------------------
template<const int STRLEN>
void Strvec<STRLEN>::clear()
{
  memset(buffer,(char)0,sizeof(char)*size*STRLEN);
  size = 0;
}

//--------------------------------------------------------
// destroy 
//--------------------------------------------------------
template<const int STRLEN>
void Strvec<STRLEN>::destroy()
{
  free(buffer);
}


#endif

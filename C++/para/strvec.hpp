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

  //return max length
  int maxlen() const {return STRLEN;}

  //reserve
  int reserve(const long len);

  //erase
  int erase(const long elm);

  //resize
  int resize(const long len);

  //find index
  long find_index(const char* name) const; 

  //compare
  int compare(const long elm, const char* name) const; 

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

//--------------------------------------------------------
// reserve 
//--------------------------------------------------------
template<const int STRLEN>
int Strvec<STRLEN>::reserve(const long len)
{
  if (capacity < len)
  {
    char* newbuf = (char*) malloc(sizeof(char)*STRLEN*len); 
    if (newbuf != NULL)
    {
      memmove(newbuf,buffer,sizeof(char)*size*STRLEN);
      memset(newbuf+size*STRLEN,(char)0,sizeof(char)*(len-size));
      capacity = len;
      free(buffer);
      buffer = newbuf; 
      return 0;
    } else {
      return 1; 
    } 
  }
  return 0;  
}

//--------------------------------------------------------
// erase -- delete an element and move the rest down 
//--------------------------------------------------------
template<const int STRLEN>
int Strvec<STRLEN>::erase(const long elm)
{
  memmove(buffer+STRLEN*elm,buffer+STRLEN*(elm+1),sizeof(char)*STRLEN*(size-elm-1));
  memset(buffer+STRLEN*(size-1),(char)0,sizeof(char)*STRLEN);
  size--;
  return 0;
}

//--------------------------------------------------------
// resize 
//--------------------------------------------------------
template<const int STRLEN>
int Strvec<STRLEN>::resize(const long len)
{
  char* newbuf = (char*) malloc(sizeof(char)*STRLEN*len); 
  if (newbuf != NULL)
  {
    memmove(newbuf,buffer,sizeof(char)*size*STRLEN);
    memset(newbuf+size*STRLEN,(char)0,sizeof(char)*(len-size));
    capacity = len;
    size = capacity;
    free(buffer);
    buffer = newbuf; 
    return 0;
  } else {
    return 1; 
  } 
  return 0;  
}

//--------------------------------------------------------
// find_index 
//	finds the index of a match of some string in
//	the list
//--------------------------------------------------------
template<const int STRLEN>
long Strvec<STRLEN>::find_index(const char* name) const
{
  for (long index=0;index<size;index++)
  {
    if (compare(index,name) == 0) {return index;} 
  }
  return -1;
}

//--------------------------------------------------------
// compare
//	compare string to strin at index
//--------------------------------------------------------
template<const int STRLEN>
int Strvec<STRLEN>::compare(const long index, const char* name) const
{
  return strncmp(name,buffer+STRLEN*index,STRLEN);
}

#endif

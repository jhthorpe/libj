/*----------------------------------------------------------------------
 * debug.hpp
 *  JHT, May 8, 2022 : created
 *
 * .hpp file containing routines useful for debugging
----------------------------------------------------------------------*/
#ifndef LIBJ_DEBUG_HPP
#define LIBJ_DEBUG_HPP

#include <string>
#include <stdio.h>

namespace libj
{

void breakpoint(const std::string& message)
{
  static int number=0;
  printf("breakpoint #%d ",number);
  printf(": %s ",message.c_str());
  printf("\n continue ? (c) ");
  char tf='f';
  while (true) {
    scanf("%c",&tf);
    if (tf == 'c') {break;}
  }
  number++;
}

} //end of namespace 

#endif

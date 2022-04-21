#ifndef ALIGNMENT_HPP
#define ALIGNMENT_HPP

namespace libj
{

size_t calc_alignment(const void* pointer)
{
  size_t alignment = 1;

  //2,4,8,16,32,64,128,256,512
  for (long m=2;m<=512;m*=2)
  {
    if ((long)pointer%m != 0) {return alignment;}
    alignment *= 2;
  }
  return alignment;
}

}
#endif

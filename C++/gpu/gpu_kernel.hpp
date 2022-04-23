/*--------------------------------------------------------------------------
  gpu_kernel.hpp
	JHT, April 17, 2022 : created

  .hpp file for GPU_KERNEL
--------------------------------------------------------------------------*/
#ifndef GPU_KERNEL_HPP
#define GPU_KERNEL_HPP

#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string>
#include <algorithm>
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

#include "gpu_include.h"
#include "gpu_program.hpp"

namespace libj
{

/*--------------------------------------------------------------------------
  GPU_KERNEL
--------------------------------------------------------------------------*/
class GPU_KERNEL
{
  public:
  //Data
  cl_kernel kernel;

  void create(const GPU_PROGRAM& program, const char* name);
  void set_arg(const int arg_num, const size_t bytes, const void* value);

};

//--------------------------------------------------------------------------
// create
//	crate the kernel as the name of a function in the program
//--------------------------------------------------------------------------
void GPU_KERNEL::create(const GPU_PROGRAM& program, const char* name)
{
  cl_int err;
  kernel = clCreateKernel(program.program,name,&err);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_KERNEL::create failed with code %d\n",err);
    exit(1);
  }
}

//--------------------------------------------------------------------------
// set_arg
//	sets the arguements for the kernel
//--------------------------------------------------------------------------
void GPU_KERNEL::set_arg(const int arg_num, const size_t bytes, const void* pointer)
{
  cl_uint arg = arg_num;
  cl_int err = clSetKernelArg(kernel,arg,bytes,pointer);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_KERNEL::set_arg failed for arg #%d with code %d\n",
           arg_num,err);
    if (err == CL_INVALID_KERNEL) {printf("Invalid kernel\n");} 
    if (err == CL_INVALID_ARG_INDEX) {printf("Invalid arg index\n");} 
    if (err == CL_INVALID_ARG_VALUE) {printf("Invalid arg value\n");} 
    exit(1);
  }
}

}//end libj namespace


#endif

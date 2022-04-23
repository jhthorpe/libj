/*------------------------------------------------------------------------------
  gpu_program.hpp
	JHt, April 17, 2022 : created

  .hpp file for a gpu_program, which manges OpenCL program and kernel 
  objects, so that they don't need to be re-compiled every run-through
------------------------------------------------------------------------------*/
#ifndef GPU_PROGRAM_HPP
#define GPU_PROGRAM_HPP

#include <stdio.h>
#include <vector>
#include <string>

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

#include "gpu_platform.hpp"
#include "gpu.hpp"
#include "gpu_include.h"

namespace libj
{

/*------------------------------------------------------------------------------
  GPU_PROGRAM
------------------------------------------------------------------------------*/
class GPU_PROGRAM
{
  private:

  public:
    cl_program               program;
    cl_kernel                kernel;
//    std::vector<cl_kernel>   kernel;
//    std::vector<std::string> kernel_names;
  
    void load(const GPU_PLATFORM& platform, const char* source); 
    void build(const GPU_PLATFORM& platform, const char* options);
    void create_kernel(const char* name);
};

/*------------------------------------------------------------------------------
  load
 	loads program from source file
------------------------------------------------------------------------------*/
void GPU_PROGRAM::load(const GPU_PLATFORM& platform, const char* source)
{
  cl_int err;
  program = clCreateProgramWithSource(platform.context,1,
                                      (const char**) &source,NULL,&err);
  if (err != CL_SUCCESS)
  { 
    printf("ERROR libj::GPU_PROGRAM::load failed to create program \n");
    printf("Error code %d \n",err);
    exit(1);
  }
}

/*------------------------------------------------------------------------------
  build
  	builds (compiles and links) the program
------------------------------------------------------------------------------*/
void GPU_PROGRAM::build(const GPU_PLATFORM& platform, const char* options)
{
  cl_int err;
  err = clBuildProgram(program,platform.num_gpu,platform.devices,
                       options,NULL,NULL);
  if (err != CL_SUCCESS)
  {
    size_t len;
    char buffer[2048];

    printf("Error libj::GPU_PROGRAM::build Failed to build program executable!\n");
    printf("Error code %d \n",err);
    clGetProgramBuildInfo(program, platform.devices[0],
                          CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
    printf("%s\n", buffer);
    exit(1);
  }

}

/*------------------------------------------------------------------------------
  create_kernel
	creates the kernal for a particular part of the current program
------------------------------------------------------------------------------*/
void GPU_PROGRAM::create_kernel(const char* name)
{
  cl_int err;
  kernel = clCreateKernel(program,name,&err);
  if (err != CL_SUCCESS)
  { 
    printf("ERROR libj::GPU_PROGRAM::create_kernel failed to create kernel\n");
    printf("Error code %d \n",err);
    exit(1);
  }
}

}//end namespace


#endif

/*--------------------------------------------------------------------------
  gpu_handler.hpp
	JHT, April 14, 2022 : created

  .hpp file for the GPU handler

  Each GPU is contained within a class called GPU. Each GPU manages it's 
  own buffers and programs

  This is currently coded only to work with the platform with the largest
  number of GPUs on it. 

  Program and kernel are currently stored in this GPU_HANDLER, but could 
  probably be moved to the specific plaform...?

  Buffers are created on the GPU_HANDLER object, which returns an 
  int to identify the buffer for the calling program

--------------------------------------------------------------------------*/
#ifndef GPU_HANDLER_HPP
#define GPU_HANDLER_HPP
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
#include "gpu_platform.hpp"
#include "gpu.hpp"
#include "gpu_program.hpp"
#include "gpu_kernel.hpp"

namespace libj
{
/*------------------------------------------------------------------------
 GPU_HANDLER
    handles the interface with OpenCL
------------------------------------------------------------------------*/
class GPU_HANDLER
{
  private:
    void init_platforms();
    void init_gpus();
    void init_context();

  public:
  //Platform data
  cl_uint                         num_platforms;
  libj::GPU_PLATFORM              platform; //the "master" platform
  std::vector<libj::GPU_PLATFORM> platforms;

  //GPU data
  cl_uint                 num_gpu; 
  std::vector<libj::GPU>  gpus;

  //Program data
  libj::GPU_PROGRAM program;

  //Kernel Data
  libj::GPU_KERNEL kernel;

  //Buffers
  std::vector<cl_mem> buffers;
  
  //Initialization 
   GPU_HANDLER();
//  ~GPU_HANDLER();

  //GPU data functions
  int get_num_gpu() const {return (int) gpus.size();}
  void print_gpu_info() const; 

  //Program functions
  void load_program(const char* source);

  //Buffer functions
  int add_buffer(const cl_mem_flags flags, const size_t bytes,
                 void* pointer);

  /*
  void enqueue_write(const int buffer_id, const cl_bool blocking, 
                     const size_t offset, const size_t size, const void* host_pointer)
  {
    err = clEnqueueWriteBuffer(commands,buffers[buffer_id],blocking,offset,size,host_pointer,
                               0,NULL,NULL);
    assert(err == CL_SUCCESS); 
  }
  */

};


//--------------------------------------------------------------------------
// Constructor
//      Initializes the GPU information
//--------------------------------------------------------------------------
GPU_HANDLER::GPU_HANDLER()
{
  printf("Initializing OpenCL enviroment\n");
  
  //get the plaform vector
  init_platforms();

  //Initialize the GPUs
  init_gpus();

  //set the context
  init_context();

}

//--------------------------------------------------------------------------
// Get platforms 
//   gets the platforms vector 
//--------------------------------------------------------------------------
void GPU_HANDLER::init_platforms()
{
  //Find the list of platforms with GPUs
  cl_platform_id tmp[MAX_NUM_PLATFORMS];
  cl_int err = clGetPlatformIDs(MAX_NUM_PLATFORMS,tmp,&num_platforms); 
  if (err != CL_SUCCESS || num_platforms <= 0)
  {
    printf("ERROR GPU_HANDLER::get_platforms failed \n"); exit(1);
  }
  num_platforms = std::min(num_platforms,(cl_uint) MAX_NUM_PLATFORMS);
  platforms.reserve(num_platforms);

  //create the list of GPU platforms
  for (int plat=0;plat<num_platforms;plat++)
  {
    libj::GPU_PLATFORM platform;
    platform.set_platform(tmp[plat],plat);
    platform.set_devices();
    platforms.push_back(platform);
  }

}

//--------------------------------------------------------------------------
// init_gpus
//	initialize the vector of GPU datastructures
//
//	Currently, this just sets the GPU list as the devices on the 
//	platforms with the most GPUs
//--------------------------------------------------------------------------
void GPU_HANDLER::init_gpus()
{
  //set the devices on the platforms
  int best=0;
  num_gpu = 0;
  for (int plat=0;plat<num_platforms;plat++)
  {
    int tmp = platforms[plat].num_gpu;
    if (tmp > num_gpu) {best = plat;num_gpu = tmp;} 
  }

  //set platform to the one with the largest number of GPU
  platform = platforms[best]; 

  //reserve the GPU Data 
  gpus.reserve(num_gpu);

  //Loop through devices on platform
  for (int dev=0;dev<num_gpu;dev++)
  {
    //get data for the GPU
    libj::GPU gpu;
    gpu.set_device(platform.devices[dev],dev);
    gpu.platform_num = best; 

    //initialize GPU data
    gpu.set_data();
 
    //push the GPU to the back of the stack
    gpus.push_back(gpu); 
  }
}

//--------------------------------------------------------------------------
// print_gpu_info
//--------------------------------------------------------------------------
void GPU_HANDLER::print_gpu_info() const
{
  printf("Printing information about GPU(s)\n");
  printf("There are %u gpus \n",get_num_gpu());
  for (int gpu=0;gpu<get_num_gpu();gpu++)
  {
    gpus[gpu].print_info();
  }
}

//--------------------------------------------------------------------------
// init_context 
//	creates context for the best platform
//--------------------------------------------------------------------------
void GPU_HANDLER::init_context()
{
  platform.set_context();

  //Now that we have context, set command queues on GPUs
  for (int dev=0;dev<num_gpu;dev++)
  {
    gpus[dev].create_command_queue(platform);
  }

}

//--------------------------------------------------------------------------
// load_program
//	loads a program from a string, creates the program with default
//	options. This program uses the context on the main platform 
//--------------------------------------------------------------------------
void GPU_HANDLER::load_program(const char* source)
{
  //load the program
  program.load(platform,source);

  //build the program
  program.build(platform,NULL);
}

//--------------------------------------------------------------------------
// Add buffer on GPU
//	add a buffer for a context 
//--------------------------------------------------------------------------
int GPU_HANDLER::add_buffer(const cl_mem_flags flags, 
                            const size_t bytes, void* pointer)
{
  cl_int err;
  cl_mem buf = clCreateBuffer(platform.context,flags,bytes,pointer,&err);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_HANDLER::add_buffer failed with code %u \n",err);
    exit(1);
  }
  buffers.push_back(buf);
  return (int) buffers.size(); 
}

}//end libj namespace
#endif

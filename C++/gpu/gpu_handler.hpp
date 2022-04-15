/*--------------------------------------------------------------------------
  gpu_handler.hpp
	JHT, April 14, 2022 : created

  .hpp file for the GPU handler
--------------------------------------------------------------------------*/
#ifndef GPU_HANDLER_HPP
#define GPU_HANDLER_HPP
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string>

#include "gpu.hpp"

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif


namespace libj
{
/*------------------------------------------------------------------------
 GPU_HANDLER
    handles the interface with OpenCL
------------------------------------------------------------------------*/
class GPU_HANDLER
{
  private:
    void init_gpus();

  public:
  //Cl managed Data  -- old and maybe replace?
  cl_platform_id      platforms;
  cl_program          programs;
  cl_context          context;
  cl_uint             num_platforms;
  cl_int              err;
  cl_command_queue    commands;
  cl_kernel           kernel;

  //newer stuff that is correct
  cl_uint             num_devices;
  cl_uint             max_num_gpu=16;
  
  //GPU data
  std::vector<libj::GPU>  gpus;

  //Buffers
  char                cbuf[2048];

  //Initialization 
   GPU_HANDLER();
  ~GPU_HANDLER();

  //Getting basic information
  int get_num_gpu() const {return (int) gpus.size();}

  //printing basic info
  void print_gpu_info() const; 

/*
  void load_program(const char* source, const char* name)
  {
    //load the program
    program = clCreateProgramWithSource(context,1,(const char**) &source,NULL,&err);
    assert( err == CL_SUCCESS );

    //build the program
    err = clBuildProgram(program,1,&device,NULL,NULL,NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    //create the kernel
    kernel = clCreateKernel(program,name,&err);
    assert(err == CL_SUCCESS); 
  }

  long add_buffer(const cl_mem_flags cl_type, const size_t size, void* host_ptr)
  {
    cl_mem buf = clCreateBuffer(context,cl_type,size,host_ptr,&err);
    assert(err == CL_SUCCESS);
    buffers.push_back(buf);
    return buffers.size();
  }

  void set_kernel_arg(const cl_uint arg, const size_t size, const void* pointer)
  {
    err = clSetKernelArg(kernel,arg,size,pointer);
    assert(err == CL_SUCCESS);
  }
  */

  /*
   buffer_id		-> list index of buffer in class
   blocking		-> CL_TRUE : copy data and enque the write, memory can be reused
   offset		-> offset in bytes from start of device buffer to write
   size			-> number of bytes to write to start buffer
   pointer		-> pointer to write FROM
   num_wait_list	-> number of events in wait list
   event_wait_list	-> list of evens to complete prior to this one
   event		-> event object to keep in list
  */
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
  
  //Get a single platform
  err = clGetPlatformIDs(1,&platforms,&num_platforms);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_HANDLER::GPU_HANDLER cannot get platforms\n");
    exit(1);
  } 

  //Initialize the GPUs
  init_gpus();

}

//--------------------------------------------------------------------------
// Destructor
//      Releases the data from the GPU
//--------------------------------------------------------------------------
GPU_HANDLER::~GPU_HANDLER()
{
  /*
   //release the buffers
   for (int buf=0;buf=buffers.size();buf++)
   {
     clReleaseMemObject(buffers[buf]);
   } 
   buffers.erase;
   
   //release the CL managed buffers
   clReleaseKernel(kernel);
   clReleaseCommandQueue(commands);
   clReleaseContext(context);
   clReleaseProgram(programs);
  */
}

//--------------------------------------------------------------------------
// init_gpus
//	initialize the GPU data
//
// Data that is gathered:
//
// Memory sizes
// 
//
// Workforce info
// CL_DEVICE_MAX_COMPUTE_UNITS		-> max number of compute units (cl_uint)
// CL_DEVICE_MAX_WORK_GROUP_SIZE	-> max number work-items in work-group (size_t)
// 
// CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS	-> (cl_uint)
// 
//
/*
  //Memory data
  long global_cache_size;
  long global_cacheline_size;
  long global_mem_size;
  long local_mem_size;
  long constant_max_size;
  long max_alloc_size;
  
  //Compute parameters
  unsigned int        max_compute_units;
  size_t              max_work_group_size;
  unsigned int        max_work_item_dim;
  std::vector<size_t> max_work_item_sizes

*/

//--------------------------------------------------------------------------
// init_gpus
//	initialize the vector of GPU datastructures
//--------------------------------------------------------------------------
void GPU_HANDLER::init_gpus()
{
  //Temporary device array
  cl_device_id devices[max_num_gpu];

  //Get the number of GPUs
  err = clGetDeviceIDs(platforms,CL_DEVICE_TYPE_GPU,max_num_gpu,devices,&num_devices);
  if (err != CL_SUCCESS || num_devices == 0)
  {
    printf("ERROR libj::GPU_HANDLER::GPU_HANDLER could not get GPU list\n");
    exit(1);
  }

  //make sure we don't have more devices than we've set the max for
  if (num_devices > max_num_gpu) {num_devices = max_num_gpu;}

  //reserve the GPU Data 
  gpus.reserve(num_devices);

  //Loop through devices, load their information
  for (int dev=0;dev<num_devices;dev++)
  {
    //get data for the GPU
    libj::GPU gpu;
    gpu.set_device(*(devices+dev));
    gpu.set_data();
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
    printf("Information about GPU #%d \n",gpu);
    gpus[gpu].print_info();
  }
}

}//end libj namespace
#endif

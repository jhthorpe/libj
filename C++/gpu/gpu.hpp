/*--------------------------------------------------------------------------
  gpu.hpp
	JHT, April 14, 2022 : created

  .hpp file for the GPU struct, which manages data invoved with 
  various gpus
--------------------------------------------------------------------------*/
#ifndef GPU_HPP
#define GPU_HPP
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <string>

#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif

#include "gpu_include.h"
#include "gpu_platform.hpp"
#include "gpu_kernel.hpp"


namespace libj
{

/*------------------------------------------------------------------------
 GPU 
  struct containing data related to each GPU

Description of data available

---- memory parameter ----
CL_DEVICE_GLOBAL_MEM_CACHE_SIZE (cl_ulong)
Size of global memory cache in bytes.

CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE (cl_uint)
Size of global memory cache line in bytes.

CL_DEVICE_GLOBAL_MEM_SIZE (cl_ulong)
Size of global device memory in bytes.

CL_DEVICE_LOCAL_MEM_SIZE (cl_ulong)
Size of local memory arena in bytes. The minimum value is 16 KB

CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE (cl_ulong)
Max size in bytes of a constant buffer allocation. The minimum value is 64 KB.

CL_DEVICE_MAX_MEM_ALLOC_SIZE (cl_ulong)
Max size of memory object allocation in bytes. The minimum value is max (1/4th of CL_DEVICE_GLOBAL_MEM_SIZE, 128*1024*1024)

---- compute parameters ----
CL_DEVICE_MAX_COMPUTE_UNITS (cl_uint)
The number of parallel compute cores on the OpenCL device. The minimum value is 1.

CL_DEVICE_MAX_WORK_GROUP_SIZE (size_t)
Maximum number of work-items in a work-group executing a kernel using the data parallel execution model. (Refer to clEnqueueNDRangeKernel). The minimum value is 1.

CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS (cl_uint)
Maximum dimensions that specify the global and local work-item IDs used by the data parallel execution model. (Refer to clEnqueueNDRangeKernel). The minimum value is 3.

CL_DEVICE_MAX_WORK_ITEM_SIZES (size_t[])
Maximum number of work-items that can be specified in each dimension of the work-group to clEnqueueNDRangeKernel.
Returns n size_t entries, where n is the value returned by the query for CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS. The minimum value is (1, 1, 1).

------------------------------------------------------------------------*/
struct GPU
{
  //destructor
  ~GPU();

  //Device id
  int           dev_num;	//device number
  int           platform_num;	//device platform number
  cl_device_id  device; 

  //Memory data
  // Temporarily disables. Typing problems with OpenCL standard
  unsigned long   global_cache_size;
  unsigned long   global_cacheline_size;
  unsigned long   global_mem_size;
  unsigned long   local_mem_size;
  unsigned long   constant_max_size;
  unsigned long   max_alloc_size;
  
  //true if this device supports doubles
  bool            supports_double;

  //Compute parameters
  cl_uint             max_compute_units;
  size_t              max_work_group_size;
  cl_uint             max_work_item_dim;
  std::vector<size_t> max_work_item_sizes;
  
  //OpenCL objects
//  cl_platform_id      platform;
  cl_command_queue      commands;

  //Function to print info
  void print_info() const; 

  //Functions to set values
  void set_device(const cl_device_id device, const int num);
  void set_data();
  void set_global_cache_size();
  void set_global_cacheline_size();
  void set_global_mem_size();
  void set_local_mem_size();
  void set_constant_max_size();
  void set_max_alloc_size();
  void set_max_compute_units();
  void set_max_work_group_size();
  void set_max_work_item_dim();
  void set_max_work_item_sizes();
  void set_supports_double();
//  void set_platform_id();
  unsigned long safe_set(const cl_device_info);

  //create context, program, kernel, etc
  void create_command_queue(const GPU_PLATFORM& platform);

  //queue a command
  void queue_command(const GPU_KERNEL& kernel,const size_t work_dim,
                     const size_t* global_work_size_array, 
                     const size_t* local_work_size_array);

  //reading and writing buffers
  void read_to_host(const cl_mem buffer, 
                    const size_t offset, const size_t bytes, 
                    void* pointer);
  
};

//-----------------------------------------------------------------------
// destructor
//-----------------------------------------------------------------------
GPU::~GPU()
{
//  clReleaseCommandQueue(commands); 
/*
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  */
}

//-----------------------------------------------------------------------
// print_info
//	prints the GPU info
//-----------------------------------------------------------------------
void GPU::print_info() const
{
  printf("GPU Memory Parameters on GPU #%d\n",dev_num);
  printf("Global Cache Size     (bytes)  %lu \n",global_cache_size);  
  printf("Global CacheLine Size (bytes)  %lu \n",global_cacheline_size);  
  printf("Global Memory Size    (bytes)  %lu \n",global_mem_size);
  printf("Local Memory size     (bytes)  %lu \n",local_mem_size);
  printf("Max Const buffer size (bytes)  %lu \n",constant_max_size);
  printf("Max allocatable size  (bytes)  %lu \n",max_alloc_size);
  printf("\n");
  printf("GPU Group Parameters\n");
  printf("Max compute units     (int)    %u \n",max_compute_units); 
  printf("Max work group size   (int)    %zu \n",max_work_group_size);
  printf("Max work item Dim.    (int)    %u \n",max_work_item_dim);
  printf("Max work item sizes   (int)    %zu",max_work_item_sizes[0]);
  for (int dim=1;dim<max_work_item_dim;dim++)
  {
  printf(" %zu",max_work_item_sizes[dim]);
  }
  printf("\n");
}

//-----------------------------------------------------------------------
// set_device
//-----------------------------------------------------------------------
void GPU::set_device(const cl_device_id dev, const int num)
{
  device = dev;
  dev_num = num;
}

//-----------------------------------------------------------------------
// set_data
//-----------------------------------------------------------------------
void GPU::set_data()
{
//  set_platform_id();
  set_global_cache_size();
  set_global_cacheline_size();
  set_global_mem_size();
  set_local_mem_size();
  set_constant_max_size();
  set_max_alloc_size();
  set_max_compute_units();
  set_max_work_group_size();
  set_max_work_item_dim();
  set_max_work_item_sizes();
  set_supports_double();
}


//-----------------------------------------------------------------------
// safe_set
//	OpenCL sometimes doesn't return a value that is the size
//	described in it's standard. This routines handles the typing
//	for you 
//-----------------------------------------------------------------------
unsigned long GPU::safe_set(const cl_device_info info)
{
  size_t actual_bytes;
  cl_int err = clGetDeviceInfo(device,info,
                               sizeof(unsigned long),
                               NULL,
                               &actual_bytes);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU::safe_set could not retrieve actual bytes on GPU #%d\n",dev_num);
    exit(1);
  }

  if (actual_bytes == 4)
  {
    unsigned int size4;
    err = clGetDeviceInfo(device,info,
                          sizeof(size4),
                          &size4,NULL);
    if (err != CL_SUCCESS)
    {
      printf("ERROR libj::GPU::safe_set could not get device info on GPU #%d\n",dev_num);
      exit(1);
    }
    return (unsigned long) size4;

  } else if (actual_bytes == 8) {
    unsigned long size8;
    err = clGetDeviceInfo(device,info,
                          sizeof(size8),
                          &size8,NULL);
    if (err != CL_SUCCESS)
    {
      printf("ERROR libj::GPU::safe_set could not get device info on GPU #%d\n",dev_num);
      exit(1);
    }
    return (unsigned long) size8;

  } else if (actual_bytes == 16) {
    unsigned long long size16;
    err = clGetDeviceInfo(device,info,
                          sizeof(size16),
                          &size16,NULL);
    if (err != CL_SUCCESS)
    {
      printf("ERROR libj::GPU::safe_set could not get device info on GPU #%d\n",dev_num);
      exit(1);
    }
    return (unsigned long) size16;

  } else {
    printf("ERROR libj::GPU::safe_set is not coded to work with %zu byte values \n",
           actual_bytes);
    exit(1);
  } 
}

//-----------------------------------------------------------------------
// set_global_cache_size
//-----------------------------------------------------------------------
void GPU::set_global_cache_size()
{
  global_cache_size = safe_set(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE);
}

//-----------------------------------------------------------------------
// set_global_cacheline_size
//-----------------------------------------------------------------------
void GPU::set_global_cacheline_size()
{
  global_cacheline_size = safe_set(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE);
}

//-----------------------------------------------------------------------
// set_global_mem_size
//-----------------------------------------------------------------------
void GPU::set_global_mem_size()
{
  global_mem_size = safe_set(CL_DEVICE_GLOBAL_MEM_SIZE); 
}

//-----------------------------------------------------------------------
// set_local_mem_size
//-----------------------------------------------------------------------
void GPU::set_local_mem_size()
{
  local_mem_size = safe_set(CL_DEVICE_LOCAL_MEM_SIZE);
}

//-----------------------------------------------------------------------
// set_constant_max_size
//-----------------------------------------------------------------------
void GPU::set_constant_max_size()
{
  constant_max_size = safe_set(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE);
}

//-----------------------------------------------------------------------
// set_max_alloc_size
//-----------------------------------------------------------------------
void GPU::set_max_alloc_size()
{
  max_alloc_size = safe_set(CL_DEVICE_MAX_MEM_ALLOC_SIZE);
}

//-----------------------------------------------------------------------
//  set_supports_double
//	determines if the GPU supports doubles
//	based on the length of CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
//-----------------------------------------------------------------------
void GPU::set_supports_double()
{
  cl_uint width = safe_set(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE);
  supports_double = true ? width != 0 : false;
}
 

/*
//-----------------------------------------------------------------------
// set_platform_id
//-----------------------------------------------------------------------
void GPU::set_platform_id()
{
  cl_int err;
  err = clGetDeviceInfo(device,CL_DEVICE_PLATFORM,
                        sizeof(cl_platform_id),
                        &platform,
                        NULL);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU::set_platform_id failted on GPU #%d\n",dev_num);
    exit(1);
  }
}
*/

//-----------------------------------------------------------------------
// set_max_compute_units
//-----------------------------------------------------------------------
void GPU::set_max_compute_units()
{
  cl_int err = clGetDeviceInfo(device,CL_DEVICE_MAX_COMPUTE_UNITS,
                               sizeof(max_compute_units),
                               &max_compute_units,
                               NULL);
  if (err != CL_SUCCESS) 
  {
    printf("ERROR libj::GPU::set_max_compute_units failed on GPU #%d\n",dev_num);
    exit(1);
  }
}

//-----------------------------------------------------------------------
// set_max_work_group_size
//-----------------------------------------------------------------------
void GPU::set_max_work_group_size()
{
  cl_int err = clGetDeviceInfo(device,CL_DEVICE_MAX_WORK_GROUP_SIZE,
                               sizeof(max_work_group_size),
                               &max_work_group_size,
                               NULL);
  if (err != CL_SUCCESS) 
  {
    printf("ERROR libj::GPU::set_max_work_group_size failed for GPU #%d\n",dev_num);
    exit(1);
  }
}

//-----------------------------------------------------------------------
// set_max_work_item_dim
//-----------------------------------------------------------------------
void GPU::set_max_work_item_dim()
{
  cl_int err = clGetDeviceInfo(device,CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                               sizeof(max_work_item_dim),
                               &max_work_item_dim,
                               NULL);
  if (err != CL_SUCCESS) 
  {
    printf("ERROR libj::GPU::set_max_work_item_dim failed for GPU #%d\n",dev_num);
    exit(1);
  }
}

//-----------------------------------------------------------------------
// set_max_work_item_sizes
//-----------------------------------------------------------------------
void GPU::set_max_work_item_sizes()
{
  size_t* tmp = (size_t*) malloc(sizeof(size_t)*max_work_item_dim);

  cl_int err = clGetDeviceInfo(device,CL_DEVICE_MAX_WORK_ITEM_SIZES,
                               sizeof(size_t)*max_work_item_dim,
                               tmp,
                               NULL);
  if (err != CL_SUCCESS) 
  {
    printf("ERROR libj::GPU::set_max_work_item_sizes failed on GPU #%d\n",dev_num);
    exit(1);
  }

  max_work_item_sizes.reserve(max_work_item_dim);
  for (int dim=0;dim<max_work_item_dim;dim++)
  {
    max_work_item_sizes.push_back(tmp[dim]); 
  }

  free(tmp);

}

//-----------------------------------------------------------------------
// create_command_queue
//   create the command queue
//	this takes the default options for now
//-----------------------------------------------------------------------
void GPU::create_command_queue(const GPU_PLATFORM& platform)
{
  cl_int err;
  commands = clCreateCommandQueue(platform.context,device,0,&err);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU::create_command_queue failed on GPU #%d\n",dev_num);
    exit(1);
  } 
}

//-----------------------------------------------------------------------
// queue the command for right-away execution
//-----------------------------------------------------------------------
void GPU::queue_command(const GPU_KERNEL& kernel,const size_t work_dim,
                        const size_t* global_work_size_array, 
                        const size_t* local_work_size_array)
{
  cl_int err = clEnqueueNDRangeKernel(commands,kernel.kernel,work_dim,
                                      NULL,global_work_size_array,
                                      local_work_size_array,
                                      0,NULL,NULL);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU::queue_command failed with error %d \n",err);
    exit(1);
  }
}

//-----------------------------------------------------------------------
// read_to_host
//	read a buffer to host memory from device
//-----------------------------------------------------------------------
void GPU::read_to_host(const cl_mem buffer, const size_t offset, const size_t bytes, 
                       void* pointer)
{
  cl_int err = clEnqueueReadBuffer(commands,buffer,CL_TRUE,offset,bytes,
                                   pointer,0,NULL,NULL);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU::read_to_host failed with status %d \n",err);
    exit(1);
  }
}

}//end namespace
#endif

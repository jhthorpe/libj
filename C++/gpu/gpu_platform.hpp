/*-------------------------------------------------------------------------
  gpu_platform.hpp
	JHT, April 15, 2022 : created

  .hpp file that handles the data related to a single GPU platform
-------------------------------------------------------------------------*/
#ifndef GPU_PLATFORM_HPP
#define GPU_PLATFORM_HPP

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

namespace libj
{

class GPU_PLATFORM
{
  public:
  //Destructor
  ~GPU_PLATFORM();

  //Basic Data
  int             platform_num;
  cl_platform_id  platform;
  cl_uint         num_gpu; 
  cl_device_id    devices[MAX_NUM_GPU];
  cl_context      context;
  
  //set basic data
  void set_platform(const cl_platform_id plat, const int plat_num);
  void set_devices();
  void set_context();
  
};

//-------------------------------------------------------------------------
// Destructor
//-------------------------------------------------------------------------
GPU_PLATFORM::~GPU_PLATFORM()
{
  //I do not understand why this causes seg fault but it do...
 // clReleaseContext(context);
}

//-------------------------------------------------------------------------
// set_platform
//-------------------------------------------------------------------------
void GPU_PLATFORM::set_platform(const cl_platform_id plat, const int plat_num)
{
  platform = plat;
  platform_num = plat_num;
}

//-------------------------------------------------------------------------
// set_devices
//	discover the devices on the platform
//-------------------------------------------------------------------------
void GPU_PLATFORM::set_devices()
{
  cl_int err = clGetDeviceIDs(platform,CL_DEVICE_TYPE_GPU,MAX_NUM_GPU,
                              devices,&num_gpu);  
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_PLATFORM::set_devices failed on platform #%d\n",
            platform_num);
    exit(1);
  }

  num_gpu = std::min(num_gpu,(cl_uint) MAX_NUM_GPU);
}

//-------------------------------------------------------------------------
// set_context
//	sets the context for this platform
//-------------------------------------------------------------------------
void GPU_PLATFORM::set_context()
{
  cl_int err;
  context = clCreateContext(0,num_gpu,devices,NULL,NULL,&err);
  if (err != CL_SUCCESS)
  {
    printf("ERROR libj::GPU_PLATFORM::set_context failed on platform #%d\n",
           platform_num);
    exit(1);
  }
} 

}//end namespace

#endif

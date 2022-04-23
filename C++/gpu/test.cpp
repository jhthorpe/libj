#include <stdio.h>
#include "gpu_handler.hpp"

//Specific kernal to use
#include "axpy_kernel.h"
#include "set_kernel.h"

int main()
{
  const int N=10;
  double a=0.5;
  double X[N];
  double Y[N];

  //This initializes the GPU handler and gpus
  libj::GPU_HANDLER GPU;
  GPU.print_gpu_info();

  if (GPU.gpus[0].supports_double) {printf("YES DOUBLE!!\n");}
  if (!GPU.gpus[0].supports_double) {printf("NO DOUBLE :(\n");}

  //load the program
  GPU.load_program(dset); 

  //add buffer, by default this is blocking
  int X_buf_id = GPU.add_buffer(CL_MEM_READ_WRITE,sizeof(double)*N,NULL);

  cl_int err;
  cl_mem buff = clCreateBuffer(GPU.platform.context,CL_MEM_READ_WRITE,N*sizeof(double),NULL,&err);
  if (err != CL_SUCCESS) {printf("create buffer errored \n");}

  //Set kernel and arguments
  //create a kernal with arguments  
  libj::GPU_KERNEL kernel;
  kernel.create(GPU.program,"dset");
  //kernel.set_arg(1,sizeof(GPU.buffers[X_buf_id]),(void*) GPU.buffers[X_buf_id]);
  kernel.set_arg(0,sizeof(buff),(void*)&buff);
//  kernel.set_arg(1,sizeof(double),(void*) &a);

  for (int i=0;i<N;i++)
  {
    X[i] = 10;
  }

  //call the command on just one GPU
  size_t global_work_size = 6;
  GPU.gpus[0].queue_command(kernel,1,&global_work_size,NULL); 

  //read memory back
  GPU.gpus[0].read_to_host(buff,0,sizeof(double)*N,X); 

  //return
  clFinish(GPU.gpus[0].commands);

  //test
  for (int i=0;i<N;i++)
  {
    if (X[i] != a) {printf("a[%d] != %lf | %lf\n",i,a,X[i]);}
  }

  //try via map 
  double* ptr;
  ptr = (double*) clEnqueueMapBuffer(GPU.gpus[0].commands,buff,CL_TRUE,CL_MAP_READ,0,N*sizeof(double),0,NULL,NULL,NULL); 
  for (int i=0;i<N;i++)
  {
    printf("val = %lf \n",*ptr);
    ptr++;
  }

   
  return 0;
}

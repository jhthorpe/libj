#include <stdio.h>
#include "gpu_handler.hpp"

//Specific kernal to use
#include "axpy_kernel.h"

int main()
{
  const int N=1048;
  double a=0.5;
  double X[N];
  double Y[N];

  libj::GPU_HANDLER GPU;

  printf("There are %d GPUs \n",GPU.get_num_gpu());
  GPU.print_gpu_info();

/*
  //Initialize the handler
  GPU.init();

  //load the program
  GPU.load_program(axpy,"daxpy"); 

  //make buffers, this copies them over as well 
  //This one is a little fiddly, as my Mac uses L3 for the GPU 
  //
  // Note that it is probably better to load a big buffer on the GPU and 
  //   then just use it as needed, rather than allocating per program
  int X_buf_id = GPU.add_buffer(CL_MEM_READ_ONLY,sizeof(double)*N,NULL); //buffer for X
  int Y_buf_id = GPU.add_buffer(CL_MEM_READ_WRITE,sizeof(double)*N,NULL); //buffer for Y

  //send X and Y to the buffers (could have done this in previous step)
  GPU.enqueue_write(X_buf_id,CL_TRUE,0,sizeof(double)*N,X);
  GPU.enqueue_write(Y_buf_id,CL_TRUE,0,sizeof(double)*N,Y);

  //set arguements for kernel
  GPU.set_kernel_arg(0,sizeof(double),&a);
  GPU.set_kernel_arg(1,sizeof(cl_mem),GPU.buffers[X_buf_id]);
  GPU.set_kernel_arg(2,sizeof(cl_mem),&GPU.buffers[Y_buf_id]);
 
  //Query the kernal working group size
//  GPU.query(CL_KERNEL_WORK_GROUP_SIZE,); 
*/
  

   
  return 0;
}

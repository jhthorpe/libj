//Global program string
#ifdef cl_khr_fp64
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
#elif defined(cl_amd_fp64)
#pragma OPENCL EXTENSION cl_amd_fp64 : enable
#else
#error "Double precision floating point not supported by OpenCL implementation."
#endif
const char* fset = "\n"
"__kernel void dset(__global float* x)\n"
"{\n"
"  x[get_global_id(0)] = 42;\n"
"}\n";

const char* dset = "\n"
"__kernel void dset(__global double* x)\n"
"{\n"
"  x[get_global_id(0)] = 33;\n"
"}\n";

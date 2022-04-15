//Global program string
const char* axpy =  "\n"
"__kernel void daxpy(const double a, const __global double* x, \n"
"                   __global double* y) \n"
"{\n"
"  uint gid = get_global_id(0);\n"
"  y[gid] = a* x[gid] + y[gid];\n"
"}\n";


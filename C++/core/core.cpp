/*-------------------------------------------------------
  Core.cpp 
	Class for dealing with Core memory
--------------------------------------------------------*/
#include "core.hpp"

/*-------------------------------------------------------
  Constructors
    - internal routine to construct Core
  
  const long	: n, number of elements to allocate 
-------------------------------------------------------*/
template<typename T>
Core<T>::Core() : len{0}, allocated{false}, assigned{false} 
{
}
template Core<double>::Core();
template Core<float>::Core();
template Core<long>::Core();
template Core<int>::Core();
template Core<double*>::Core();

template<typename T>
Core<T>::Core(const long n) 
{
  allocated = false;
  assigned = false;
  allocate(n);
}
template Core<double>::Core(const long n);
template Core<float>::Core(const long n);
template Core<long>::Core(const long n);
template Core<int>::Core(const long n);
template Core<double*>::Core(const long n);

template<typename T>
Core<T>::Core(const long n, T* ptr) 
{
  allocated = false;
  assigned = false;
  assign(n,ptr);
}
template Core<double>::Core(const long n, double* ptr);
template Core<float>::Core(const long n, float* ptr);
template Core<long>::Core(const long n, long* ptr);
template Core<int>::Core(const long n, int* ptr);
template Core<double*>::Core(const long n, double** ptr);

/*-------------------------------------------------------
  Destructor 
    - internal routine to destruct Core
-------------------------------------------------------*/
template<typename T>
Core<T>::~Core()
{
  if (allocated)
  {
    deallocate();
  } else if (assigned) {
    unassign();
  }
}
template Core<double>::~Core();
template Core<float>::~Core();
template Core<long>::~Core();
template Core<int>::~Core();
template Core<double*>::~Core();

/*-------------------------------------------------------
  Allocator  
    - allocates the buffer

  const long	: n, number of elements to allocate 
-------------------------------------------------------*/
template<typename T>
void Core<T>::allocate(const long n)
{
  const long mm=std::numeric_limits<long>::max();
  if (!(allocated||assigned) && n >= 1 && n <= mm) 
  {
    buf = (T*) malloc(n*sizeof(T));
    next = buf;
    len = n;
    navbl = n;
    ntake = 0;
    allocated = true;
    assigned = false;
  } else if (allocated) {
    printf("Attempted to allocate already allocated Core \n");
    exit(1);
  } else if (assigned) {
    printf("Attempted to assign already assigned Core \n");
    exit(1);
  } else if (n < 1) {
    printf("Attempted to allocate Core of < 1 element \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to allocate Core beyond long limit \n");
    exit(1);
  }

  //Check that allocation actually worked...
  if (buf == NULL)
  {
    printf("Allocation of Core failed \n");
    exit(1);
  }
}
template void Core<double>::allocate(const long n);
template void Core<float>::allocate(const long n);
template void Core<long>::allocate(const long n);
template void Core<int>::allocate(const long n);
template void Core<double*>::allocate(const long n);

/*-------------------------------------------------------
  Deallocator
    - deallocates the buffer
-------------------------------------------------------*/
template<typename T>
void Core<T>::deallocate()
{
  if (allocated)
  {
    next = NULL;
    free(buf);
    allocated = false;
  } else {
    printf("Attempted to deallocated an unallocated Core\n");
    exit(1);
  }
}
template void Core<double>::deallocate();
template void Core<float>::deallocate();
template void Core<long>::deallocate();
template void Core<int>::deallocate();
template void Core<double*>::deallocate();

/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
template<typename T>
void Core<T>::assign(const long n, T* pntr)
{
  const long mm=std::numeric_limits<long>::max();
  if (!(allocated||assigned) && n >= 1 && n <= mm && pntr != NULL) 
  {
    buf = pntr; 
    next = buf;
    len = n;
    navbl = n;
    ntake = 0;
    allocated = false;
    assigned = true;
  } else if (allocated || assigned) {
    printf("Attempted to assign an already set Core \n");
    exit(1);
  } else if (n < 1) {
    printf("Attempted to assign Core with len < 1 \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to assign Core > long limit \n");
    exit(1);
  } else if (pntr == NULL) {
    printf("Attempted to assign Core to NULL pointer \n");
    exit(1);
  } else {
    printf("Some assignment condition of Core not met \n");
    exit(1);
  }
}
template void Core<double>::assign(const long n, double* pntr);
template void Core<float>::assign(const long n, float* pntr);
template void Core<long>::assign(const long n, long* pntr);
template void Core<int>::assign(const long n, int* pntr);
template void Core<double*>::assign(const long n, double** pntr);

/*-------------------------------------------------------
  unassign 
    - unassigns Core 
-------------------------------------------------------*/
template<typename T>
void Core<T>::unassign()
{
  if (assigned)
  {
    buf = NULL;
    next = NULL;
    len = 0;
    assigned = false;
  } else {
    printf("Attempted to unassign an unassigned Core\n");
    exit(1);
  }
}
template void Core<double>::unassign();
template void Core<float>::unassign();
template void Core<long>::unassign();
template void Core<int>::unassign();
template void Core<double*>::unassign();

/*-------------------------------------------------------
  info
    - prints information about Core
-------------------------------------------------------*/
template<typename T>
void Core<T>::info() const
{
  if (allocated||assigned)
  {
    printf("Core has \n");
    printf("%ld elements \n",len);
    printf("%ld free elements \n",navbl);
    printf("%ld taken elements \n",ntake);
    printf("buffer begins at %p \n",(void*)buf);
    printf("next element  at %p \n",(void*)next);
  } else {
    printf("Core is not allocated or assigned \n");
  }
  printf("\n");
}
template void Core<double>::info() const;
template void Core<float>::info() const;
template void Core<long>::info() const;
template void Core<int>::info() const;
template void Core<double*>::info() const;

/*-------------------------------------------------------
  checkout(const long n) 
    - checks out a section of Core, and returns a 
      pointer to the location 

  const long		: n, number of elements 
-------------------------------------------------------*/
template<typename T>
T* Core<T>::checkout(const long n)
{
  if (allocated||assigned) 
  {
    if (navbl >= n) 
    {
      T* ptr = next;
      navbl -= n;
      next += n;
      return ptr;
    } else {
      printf("Attempted to checkout more elements than Core has\n");
      exit(1);
    }
  } else {
    printf("Attempted to checkout unallocated or unassigned Core \n");
    exit(1);
  }
  return NULL;
}
template double* Core<double>::checkout(const long n);
template float* Core<float>::checkout(const long n);
template long* Core<long>::checkout(const long n);
template int* Core<int>::checkout(const long n);
template double** Core<double*>::checkout(const long n);

/*------------------------------------------------------
  aligned_checkout
------------------------------------------------------*/
template<typename T>
T* Core<T>::aligned_checkout(const long ALIGN, const long n)
{
  if (allocated||assigned)
  {
    if (ALIGN%sizeof(T) == 0)
    {
      const long nextra = ALIGN/sizeof(T); //number of extra Ts
      if (navbl >= (n+nextra))
      {
        T* ptr = next;
        navbl -= (n+nextra);
        next += (n+nextra);

        long M = (long)ptr%(long)ALIGN; //number of BYTES we are off
        if (M != 0)
        {
          char* cptr = (char*) ptr;
          for (long i=0;i<(ALIGN-M);i++)
          {
            ++cptr;
          }
          return (T*) cptr;
        } else {
          return ptr;
        }
      
      } else {
        printf("Attempted to aligned checkout more elements than Core has\n");
        exit(1);
      }
      
    } else {
      printf("Attempted aligned checkout of Core with non-integer T alignment \n");
      exit(1);
    } 
     
  } else {
    printf("Attempted aligned checkout of unallcoated or unassigned Core\n");
    exit(1);
  }
  return NULL;
  
}
template double* Core<double>::aligned_checkout(const long ALIGN, const long n);
template float* Core<float>::aligned_checkout(const long ALIGN, const long n);
template long* Core<long>::aligned_checkout(const long ALIGN, const long n);
template int* Core<int>::aligned_checkout(const long ALIGN, const long n);
template double** Core<double*>::aligned_checkout(const long ALIGN, const long n);

/*-------------------------------------------------------
  remove(const long n) 
    - removes a number of elements from the end of the
      memory 

  const long		: n, number of elements 
-------------------------------------------------------*/
template<typename T>
int Core<T>::remove(const long n)
{
  if (allocated||assigned) 
  {
    navbl += n;
    if (navbl <= len )
    { 
      next -= n;
      return 0;
    } else {
      navbl = len;
      next = buf;
      return 0;
    }
  } else {
    printf("Attempted to remove unallocated or unassigned Core \n");
    exit(1);
  }
  return 1; 
}
template int Core<double>::remove(const long n);
template int Core<float>::remove(const long n);
template int Core<long>::remove(const long n);
template int Core<int>::remove(const long n);
template int Core<double*>::remove(const long n);

/*-------------------------------------------------------
  take_free(const long n)
  - takes a number of free elements from the Core memory.
    
  const long n  : number of elements
-------------------------------------------------------*/
template<typename T>
int Core<T>::take_free(const long n)
{
  if (navbl >= n)
  {
    navbl -= n;
    ntake += n;
    return 0;
  } else {
    printf("Core::take_free attempted to take more free memory than available \n");
    exit(1);
  }
  return 1;
}
template int Core<double>::take_free(const long n);
template int Core<float>::take_free(const long n);
template int Core<long>::take_free(const long n);
template int Core<int>::take_free(const long n);
template int Core<double*>::take_free(const long n);

/*-------------------------------------------------------
  give_free(const long n)
  - gives a number of free elements to the Core memory 
    
  const long n  : number of elements
-------------------------------------------------------*/
template<typename T>
int Core<T>::return_free(const long n)
{ 
  ntake = ((ntake-n) > 0) ? (ntake-n) : 0;
  navbl = ((navbl+n) < len) ? (navbl+n) : len;
  return 0;
}
template int Core<double>::return_free(const long n);
template int Core<float>::return_free(const long n);
template int Core<long>::return_free(const long n);
template int Core<int>::return_free(const long n);
template int Core<double*>::return_free(const long n);



/*-------------------------------------------------------
  dcore.cpp 
	Class for dealing with core memory


  
--------------------------------------------------------*/
#include "dcore.hpp"

/*-------------------------------------------------------
  Constructors
    - internal routine to construct dcore
  
  const size_t	: n, number of elements to allocate 
-------------------------------------------------------*/
dcore::dcore() : len{0}, allocated{false} 
{
}

dcore::dcore(const std::size_t n) 
{
  allocate(n);
}

/*-------------------------------------------------------
  Destructor 
    - internal routine to destruct dcore
-------------------------------------------------------*/
dcore::~dcore()
{
  if (allocated)
  {
    deallocate();
  }
}

/*-------------------------------------------------------
  Allocator  
    - allocates the buffer

  const size_t	: n, number of elements to allocate 
-------------------------------------------------------*/
void dcore::allocate(const std::size_t n)
{
  const std::size_t mm=std::numeric_limits<std::size_t>::max();
  if (!allocated && n > 1 && n <= mm) 
  {
    buf = (double*) malloc(n*sizeof(double));
    next = buf;
    len = n;
    navbl = n;
    allocated = true;
  } else if (allocated) {
    printf("Attempted to allocate already allocated dcore \n");
    exit(1);
  } else if (n < 1) {
    printf("Attempted to allocate dcore of < 1 element \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to allocate dcore beyond size_t limit \n");
    exit(1);
  }

  //Check that allocation actually worked...
  if (buf == NULL)
  {
    printf("Allocation of dcore failed \n");
    exit(1);
  }
}

/*-------------------------------------------------------
  Deallocator
    - deallocates the buffer
-------------------------------------------------------*/
void dcore::deallocate()
{
  if (allocated)
  {
    next = NULL;
    free(buf);
    allocated = false;
  } else {
    printf("Attempted to deallocated an unallocated dcore\n");
    exit(1);
  }
  
}


/*-------------------------------------------------------
  info
    - prints information about dcore
-------------------------------------------------------*/
void dcore::info() const
{
  if (allocated)
  {
    printf("dcore has \n");
    printf("%zu elements \n",len);
    printf("%zu free elements \n",navbl);
    printf("buffer begins at %p \n",buf);
    printf("next element  at %p \n",next);
  } else {
    printf("dcore is not allocated \n");
  }
  printf("\n");
}

/*-------------------------------------------------------
  size()
	- returns the size of the array
-------------------------------------------------------*/
std::size_t dcore::size() const {return len;}

/*-------------------------------------------------------
  nfree()
	- returns the size of number of free elements
-------------------------------------------------------*/
std::size_t dcore::nfree() const {return navbl;}

/*-------------------------------------------------------
  checkout(const size_t n) 
    - checks out a section of dcore, and returns a 
      pointer to the location 

  const size_t		: n, number of elements 
-------------------------------------------------------*/
double* dcore::checkout(const std::size_t n)
{
  if (allocated) 
  {
    if (navbl >= n) 
    {
      navbl -= n;
      next += n;
    } else {
      printf("Attempted to checkout more elements than dcore has\n");
      exit(1);
    }
  } else {
    printf("Attempted to checkout unallocated dcore \n");
    exit(1);
  }
  return next;
}

/*-------------------------------------------------------
  remove(const size_t n) 
    - removes a number of elements from the end of the
      memory 

  const size_t		: n, number of elements 
-------------------------------------------------------*/
double* dcore::remove(const std::size_t n)
{
  if (allocated) 
  {
    navbl += n;
    if (navbl <= len )
    { 
      next -= n;
    } else {
      navbl = len;
      next = buf;
    }
  } else {
    printf("Attempted to remove unallocated dcore \n");
    exit(1);
  }
  return next;
}
/*-------------------------------------------------------
  () accessing 
  const size_t		: i, number of elements 
-------------------------------------------------------*/
double& dcore::operator() (const std::size_t i)
{
  return(*(buf+i));
}

double dcore::operator() (const std::size_t i) const
{
  return (*(buf+i));
}

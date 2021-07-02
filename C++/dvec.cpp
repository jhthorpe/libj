/*-------------------------------------------------------
  dvec.cpp
  .cpp file for dvec class, desinged to deal with 
  double precision vecotrs

  To Use
  
-------------------------------------------------------*/
#include "dvec.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
dvec::dvec() 
{
  assigned = false;
  allocated = false;
  len = 0;
  buf = NULL;
}

dvec::dvec(const std::size_t n)
{
  assigned = false;
  allocated = false;
  allocate(n);
}

dvec::dvec(const std::size_t n, double* ptr)
{
  assigned = false;
  allocated = false;
  assign(n,ptr);
}
/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
dvec::~dvec()
{ 
  if (assigned)
  { 
    unassign();
  } else if (allocated)
  {
    deallocate();
  }
}

/*-------------------------------------------------------
  Allocate 
    - uses malloc to allocate an double prescision 
      vector
-------------------------------------------------------*/
void dvec::allocate(const std::size_t n)
{
  const std::size_t mm=std::numeric_limits<std::size_t>::max();
  if (!(allocated || assigned) && n > 1 && n <= mm) 
  {
    len = n;
    buf = (double*) malloc(n*sizeof(double));
    allocated = true; 
    assigned = false;
  } else if (n < 1) {
    printf("Attempted to allocate dvec of < 1 element \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to allocated dvec > size_t limit \n");
    exit(1);
  } else if (allocated || assigned) {
    printf("Attempted to allocate an already set dvec \n");
    exit(1);
  }
}
/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
void dvec::assign(const std::size_t n, double* pntr)
{
  const std::size_t mm=std::numeric_limits<std::size_t>::max();
  if (!(allocated || assigned) && n < mm) 
  {
    buf = pntr;
    len = n;
    assigned = true;
    allocated = false;
  } else if (allocated || assigned) {
    printf("Attempted to assign an already set dvec \n");
    exit(1);
  } else if (n < 1) {
    printf("Attempted to assign dvec with len < 1 \n");
    exit(1);
  } else if (n > mm) {
    printf("Attempted to assign dvec > size_t limit \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
void dvec::deallocate()
{
  if (allocated)  
  { 
    len = 0;
    free(buf); 
    allocated = false;
  } else {
    printf("Attempted to deallocate unset dvec \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
void dvec::unassign()
{
  if (assigned) {
    len = 0;
    buf = NULL;
    assigned = false;
  } else {
    printf("Attempted to unassign an unset dvec \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   () operator overloading 
-------------------------------------------------------*/
double& dvec::operator() (const std::size_t i)
{
  return(*(buf+i));
}

double dvec::operator() (const std::size_t i) const
{
  return(*(buf+i));
}

/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
void dvec::info() const
{
  if (assigned || allocated) 
  {
    printf("dvec has %lu elements \n",len);
    printf("dvec points to  %p \n",buf);
  } else {
    printf("dvec is unset \n");
    printf("dvec has %lu elements \n",len);
  }
}


/*-------------------------------------------------------
   size
	- returns number of elements of vector 
-------------------------------------------------------*/
std::size_t dvec::size() const
{
  return(len);
}

/*-------------------------------------------------------
   print
	- prints elements of vector 
	- uses a fancy infinite loop to do one less 
          action per iteration
-------------------------------------------------------*/
void dvec::print() const
{
  std::size_t i=0;
  while (true)
  {
    printf("[%lu]    %18.15E \n",i,*(buf+i)); 
    if (i >= len-1) break; 
    i++;
  }
}

/*-------------------------------------------------------
   zero 
	- zeros the vector. Uses a fancy inf. loop to 
	  do one less action per iteration
-------------------------------------------------------*/
void dvec::zero()
{
  std::size_t i=0;
  const std::size_t ll=len-1;
  while (true)
  {
    *(buf+i) = 0;
    if (i >= ll) break;
    i++;
  }
}

/*-------------------------------------------------------
  dgemat.cpp
  .cpp file for dgemat class, desinged to deal with 
  double precision vecotrs

  To Use
  
-------------------------------------------------------*/
#include "dgemat.hpp"

/*-------------------------------------------------------
  Constructors
-------------------------------------------------------*/
dgemat::dgemat() 
{
  assigned = false;
  allocated = false;
  len = 0;
  nrow = 0;
  ncol = 0;
  buf = NULL;
}

dgemat::dgemat(const std::size_t n, const std::size_t m)
{
  allocated = false;
  assigned = false;
  allocate(n,m);
}

dgemat::dgemat(const std::size_t n, const std::size_t m, double* ptr)
{
  allocated = false;
  assigned = false;
  assign(n,m,ptr);
}
/*-------------------------------------------------------
  Destructors 
-------------------------------------------------------*/
dgemat::~dgemat()
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
-------------------------------------------------------*/
void dgemat::allocate(const std::size_t n, const std::size_t m)
{
  const std::size_t ll = n*m;
  const std::size_t mm=std::numeric_limits<std::size_t>::max(); //gives largest size_t 
  if (!(allocated || assigned) && ll > 1 && ll <= mm) 
  {
    len = ll;
    nrow = n;
    ncol = m;
    buf = (double*) malloc(ll*sizeof(double));
    allocated = true; 
    assigned = false;
  } else if (ll < 1) {
    printf("Attempted to allocate dgemat of < 1 element \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to allocated dgemat > size_t limit \n");
    exit(1);
  } else if (allocated || assigned) {
    printf("Attempted to allocate an already set dgemat \n");
    exit(1);
  }
}
/*-------------------------------------------------------
   Assign 
     - points the start of the array to a location
       in memory
-------------------------------------------------------*/
void dgemat::assign(const std::size_t n, const std::size_t m, double* pntr)
{
  const std::size_t ll=n*m;
  const std::size_t mm=std::numeric_limits<std::size_t>::max();
  if (!(allocated || assigned) && n < mm) 
  {
    buf = pntr;
    len = ll;
    nrow = n;
    ncol = m;
    assigned = true;
    allocated = false;
  } else if (allocated || assigned) {
    printf("Attempted to assign an already set dgemat \n");
    exit(1);
  } else if (ll < 1) {
    printf("Attempted to assign dgemat with len < 1 \n");
    exit(1);
  } else if (ll > mm) {
    printf("Attempted to assign dgemat > size_t limit \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   Deallocate
-------------------------------------------------------*/
void dgemat::deallocate()
{
  if (allocated)  
  { 
    len = 0;
    nrow = 0;
    ncol = 0;
    free(buf); 
    allocated = false;
  } else {
    printf("Attempted to deallocate unset dgemat \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   unassign 
-------------------------------------------------------*/
void dgemat::unassign()
{
  if (assigned) {
    len = 0;
    nrow = 0;
    ncol = 0;
    buf = NULL;
    assigned = false;
  } else {
    printf("Attempted to unassign an unset dgemat \n");
    exit(1);
  }
}

/*-------------------------------------------------------
   () and [] operator overloading 
-------------------------------------------------------*/
double& dgemat::operator() (const std::size_t i, const std::size_t j)
{
  return(*(buf+nrow*j+i));
}

double dgemat::operator() (const std::size_t i, const std::size_t j) const
{
  return(*(buf+nrow*j+i));
}

double& dgemat::operator[] (const std::size_t i)
{
  return(*(buf + i));
}

double dgemat::operator[] (const std::size_t i) const
{
  return(*(buf+i));
}
/*-------------------------------------------------------
   info
    - prints informaiton about the vector
-------------------------------------------------------*/
void dgemat::info() const
{
  if (assigned || allocated) 
  {
    printf("dgemat has %lu elements \n",len);
    printf("dgemat has %lu rows \n",nrow);
    printf("dgemat has %lu cols \n",ncol);
    printf("dgemat points to  %p \n",buf);
  } else {
    printf("dgemat is unset \n");
    printf("dgemat has %lu elements \n",len);
    printf("dgemat has %lu rows \n",nrow);
    printf("dgemat has %lu cols \n",ncol);
  }
}


/*-------------------------------------------------------
   size,rows,cols
	- prints number of elements in vector 
-------------------------------------------------------*/
std::size_t dgemat::size() const
{
  return(len);
}
std::size_t dgemat::rows() const
{
  return(nrow);
}
std::size_t dgemat::cols() const
{
  return(ncol);
}

/*-------------------------------------------------------
  print
	- prints elements of vector
-------------------------------------------------------*/
void dgemat::print() const
{
  std::size_t xx=0;
  for (std::size_t j=0;j<ncol;j++)
  {
    for (std::size_t i=0;i<nrow;i++)
    {
      printf("[%lu,%lu]    %18.15E \n",i,j,*(buf+xx));
      xx++;
    }
  }
}

/*-------------------------------------------------------
  zero()
	- zeros the matrix 
-------------------------------------------------------*/
void dgemat::zero()
{
  const std::size_t ll=len-1;
  std::size_t i=0;
  while (true)
  {
    *(buf+i) = 0;
    if (i >= ll) break;
    i++;
  }
}

/*-------------------------------------------------------
  I() 
	- makes the main diagonal of the matrix = 1 
        - one could do this in two setps, but 
            here we only make one pass through the 
            buffer

   example
   1 0 0      1 0 0 0      1 0 0
   0 1 0  or  0 1 0 0  or  0 1 0
   0 0 1      0 0 1 0      0 0 1
   0 0 0

-------------------------------------------------------*/
void dgemat::I()
{
  //find number of iterations to traverse
  const std::size_t nitr=(nrow < ncol) ? nrow : ncol; 
  const std::size_t nr=nrow+1;
  std::size_t i=0;
 
  //go to end of part with 1's
  std::size_t itr=1;
  while (true)
  {
    *(buf+i) = 1;
    i++;
    
    //the extra i++ here is wanted
    //make all elements up to next diag zero
    for (i=i;i<itr*nr;i++)
    {
      *(buf+i) = 0;
    }

    if (itr >= nitr) break;
    itr++; 
  }

  //zero remaining elements
  while (true)
  {
    *(buf+i) = 0;
    if (i >= len) break;
    i++;
  }
}

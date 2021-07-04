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

dgemat::dgemat(const long n, const long m)
{
  allocated = false;
  assigned = false;
  allocate(n,m);
}

dgemat::dgemat(const long n, const long m, double* ptr)
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
void dgemat::allocate(const long n, const long m)
{
  const long ll = n*m;
  const long mm=std::numeric_limits<long>::max(); //gives largest uint64_t 
  if (!(allocated || assigned) && ll >= 1 && ll <= mm) 
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
    printf("Attempted to allocated dgemat > uint64_t limit \n");
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
void dgemat::assign(const long n, const long m, double* pntr)
{
  const long ll=n*m;
  const long mm=std::numeric_limits<long>::max();
  if (!(allocated || assigned) && ll >= 1 && ll < mm) 
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
    printf("Attempted to assign dgemat > uint64_t limit \n");
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
double& dgemat::operator() (const long i, const long j)
{
  return(*(buf+nrow*j+i));
}

double dgemat::operator() (const long i, const long j) const
{
  return(*(buf+nrow*j+i));
}

double& dgemat::operator[] (const long i)
{
  return(*(buf + i));
}

double dgemat::operator[] (const long i) const
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
    printf("dgemat has %ld elements \n",len);
    printf("dgemat has %ld rows \n",nrow);
    printf("dgemat has %ld cols \n",ncol);
    printf("dgemat points to  %p \n",buf);
  } else {
    printf("dgemat is unset \n");
    printf("dgemat has %ld elements \n",len);
    printf("dgemat has %ld rows \n",nrow);
    printf("dgemat has %ld cols \n",ncol);
  }
}


/*-------------------------------------------------------
   size,rows,cols
	- prints number of elements in vector 
-------------------------------------------------------*/
long dgemat::size() const
{
  return(len);
}
long dgemat::rows() const
{
  return(nrow);
}
long dgemat::cols() const
{
  return(ncol);
}

/*-------------------------------------------------------
  print
	- prints elements of vector
-------------------------------------------------------*/
void dgemat::print() const
{
  assert(allocated||assigned);
  long xx=0;
  for (long j=0;j<ncol;j++)
  {
    for (long i=0;i<nrow;i++)
    {
      printf("[%ld,%ld]    %18.15E \n",i,j,*(buf+xx));
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
  assert(allocated||assigned);
  const long ll=len-1;
  long i=0;
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
  assert(allocated||assigned);

  //find number of iterations to traverse
  const long nitr=(nrow < ncol) ? nrow : ncol; 
  const long nr=nrow+1;
  long i=0;
 
  //go to end of part with 1's
  long itr=1;
  while (true)
  {
    *(buf+i) = 1;
    i++;
    
    //the extra i++ here is wanted
    //make all elements up to next diag zero
    while (i<itr*nr)
    {
      *(buf+i) = 0;
      i++;
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

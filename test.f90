program test
  use jlib
  use fsys
  implicit none
  
  integer(kind=8) :: nelm,i0(2)
  real(kind=8) :: DBUF(1:1000) 
  integer(kind=8) :: IBUF(1:100)
  real(kind=8) :: foo(10)
  type(dmem_type) :: dmeta 
  type(jsys_type) :: sys
  integer(kind=8) :: fid

  character(len=8) :: bar

  nelm = 1000
  call dmem_init(nelm,dmeta)
  call dmem_info(dmeta)

  call dmem_checkout(10,i0(1),dmeta)
  write(*,*) "I0(1) is",i0(1)
  call dmem_info(dmeta)
  call dmem_checkout(10,i0(2),dmeta)
  call dmem_info(dmeta)
  write(*,*) "I0(2) is",i0(2)

  write(*,*) "testing len8",sizeof(bar) 
  write(*,*) "testing int*8",sizeof(nelm)

  write(*,*)
  call fsys_init(sys)
  call fsys_print(sys)
  call fsys_add(sys,'CRAP    ',5*8,fid)
  call fsys_print(sys)
  
  write(*,*)  
  DBUF(1:20) = 10.d0
  IBUF(1:3) = 5
  call fsys_open(sys,fid,1)
  call fsys_dwrite(sys,fid,1,20,DBUF(1:20))
  call fsys_iwrite(sys,fid,20*8+1,3,IBUF(1:3))
  call fsys_close_all(sys)

  write(*,*) "we have written file"
  call fsys_open(sys,fid,0)
  call fsys_dread(sys,fid,1,20,DBUF(1:20))
  call fsys_iread(sys,fid,20*8+1,3,IBUF(1:3))
  call fsys_close_all(sys)
  write(*,*) DBUF(1:20)
  write(*,*) IBUF(1:3)
  
  
end program test

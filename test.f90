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
  !EXAMPLE FOR DBUF
  call dmem_init(nelm,dmeta)
  call dmem_info(dmeta)

  call dmem_checkout(10,i0(1),dmeta)
  call dmem_info(dmeta)

  !EXAMPLE FOR FSYS
  call fsys_init(sys)
  call fsys_print(sys)
  call fsys_add(sys,'FILE1   ',5*8,fid)
  call fsys_add(sys,'FILE2   ',10*8,fid)
  write(*,*) "fid is",fid
  fid = fsys_getid(sys,'FILE2   ')
  write(*,*) "getid returned",fid
  call fsys_print(sys)
  
  write(*,*)  
  DBUF(1:20) = 10.d0
  IBUF(1:3) = 5
  call fsys_open(sys,fid,1)
  call fsys_dwrite(sys,fid,1,19,DBUF(1:19))
  call fsys_iwrite(sys,fid,20*8+1,3,IBUF(1:3))
  call fsys_close_all(sys)

  write(*,*) "we have written file"
  call fsys_open(sys,fid,0)
  call fsys_dread(sys,fid,1,19,DBUF(1:19))
  call fsys_iread(sys,fid,20*8+1,3,IBUF(1:3))
  call fsys_close_all(sys)
  write(*,*) DBUF(1:20)
  write(*,*) IBUF(1:3)

  call fsys_save(sys)

  write(*,*) "fsys was saved"
 
  call fsys_recover(sys)
  write(*,*) "fsys was recovered"
  call fsys_print(sys)
  
  
end program test

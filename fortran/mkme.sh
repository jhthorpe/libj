#!/bin/bash
gfortran -c dmem.f90 -flto -O3
gfortran -c fsys.f90 -flto -O3
gfortran -c jlib.f90 -flto -O3
gfortran test.f90 *.o -fdefault-integer-8 -flto -O3

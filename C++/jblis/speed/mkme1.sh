#!/bin/bash
incdir=/Users/james.thorpe/work/libj/C++/inc
libdir=/Users/james.thorpe/work/libj/C++/lib
for i in dscalv_*.c; do
  gcc -I$incdir -O3 -funroll-loops -flto -march=native -Xpreprocessor -fopenmp -c $i
done
g++ --std=c++11 -I$incdir -O3 -funroll-loops -flto -march=native -Xpreprocessor -fopenmp -c speed.cpp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_u6.o -o test_u6.exe -lomp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_i16.o -o test_i16.exe -lomp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_i32.o -o test_i32.exe -lomp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_i64.o -o test_i64.exe -lomp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_i16_p16_nta.o -o test_i16_p16_nta.exe -lomp
g++ --std=c++11 $libdir/libj.a speed.o dscalv_i16_p16_t0.o -o test_i16_p16_t0.exe -lomp

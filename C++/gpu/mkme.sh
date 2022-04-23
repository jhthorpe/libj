#!/bin/bash
g++ --std=c++11 -c test.cpp
g++ test.o -o test.exe -framework OpenCL 
g++ --std=c++11 -c test2.cpp
g++ test2.o -o test2.exe -framework OpenCL 

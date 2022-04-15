#!/bin/bash
g++ --std=c++11 -c test.cpp
g++ test.o -o test.exe -framework OpenCL 

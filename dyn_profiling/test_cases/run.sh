#!/bin/bash

cd ../build
make
cd ../test_cases
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/dyn_pro/libDynProfilePass.so -disable-output input.bc
clang -Xclang -load -Xclang ../build/dyn_pro/libDynProfilePass.so -c input.c
clang -S -emit-llvm input.c -o input.ll
gcc -c rtlib.c
gcc input.o rtlib.o
./a.out

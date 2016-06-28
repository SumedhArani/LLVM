#!/bin/bash

cd ../build
make
cd ../test_cases
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/dyn_pro/libDynProfilePass.so -dp -disable-output input.bc
opt -load ../build/dyn_pro/libDynProfilePass.so -dp input.bc -S > out.ll
clang -S -emit-llvm input.c -o input.ll
llc out.ll
gcc out.s
./a.out

#!/bin/bash

cd ../build
make
cd ../test_cases
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/conn_comp/libConnCompPass.so -conncomp -disable-output input.bc

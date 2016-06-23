#!/bin/bash

cd ../build
make
cd ../test_cases
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/hotField/libHotFieldPass.so -hf -disable-output input.bc

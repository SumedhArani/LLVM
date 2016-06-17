#!/bin/bash

cd ../build
make
cd ../test_cases
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/g_var/libGlobalVarPass.so -gvar -disable-output input.bc

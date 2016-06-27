
#!/bin/bash

cd ../build
make
cd ../test_cases
clang -S -emit-llvm input.c -o input.ll
clang -c -emit-llvm input.c -o input.bc
opt -load ../build/dyn_pro/libDynProfilePass.so -disable-output input.bc
clang -S -emit-llvm input.c -o input.ll
echo "hello"
clang -Xclang -load -Xclang ../build/dyn_pro/libDynProfilePass.so input.c
#./a.out

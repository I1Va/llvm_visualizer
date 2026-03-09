#!/bin/bash

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug 

cmake --build build
clang -fpass-plugin=./build/libllvm_visualizer_pass.so c_examples/hello.c -emit-llvm -S -o c_examples/hello.ll

# dot -Tpng graph.dot -o graph.png
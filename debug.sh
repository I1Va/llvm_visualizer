#!/bin/bash

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug 

cmake --build build
clang -fpass-plugin=./build/libllvm_visualizer_pass.so c_examples/fact.c -O1 -emit-llvm -S -o c_examples/fact.ll

# dot -Tpng graph.dot -o graph.png
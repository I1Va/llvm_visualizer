#!/bin/bash

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release 
cmake --build build

program_name="fact"
clang++ -std=c++17 -fpass-plugin=./build/libllvm_visualizer_pass.so c_examples/${program_name}.cpp -O2 -emit-llvm -S -o c_examples/${program_name}.ll

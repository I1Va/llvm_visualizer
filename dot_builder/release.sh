#!/bin/bash
cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release 
cmake --build build

./build/dot_builder ./../info/static_info.bin ./../info/dynamic_info.bin graph.dot

dot -Tpng graph.dot -o graph.png
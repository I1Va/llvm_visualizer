#!/bin/bash

cd llsda
cmake -S . -B build 
cmake --build build 
cd ..

cd lldoter
cmake -S . -B build 
cmake --build build 
cd ..

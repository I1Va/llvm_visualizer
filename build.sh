#!/bin/bash

cd llsda
cmake -S . -B build 
cmake --build build 
cd ..
#!/bin/bash

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release 
cmake --build build

program_name="fact"
optimization="-O0"
logger="build/CMakeFiles/logger_obj.dir/src/logger.cpp.o"
# clang++ -std=c++17 -c src/logger.cpp -o logger.o -fPIC

# clang++ -std=c++17 -fpass-plugin=./build/libllvm_visualizer_pass.so c_examples/${program_name}.cpp logger.o -O2 -emit-llvm -S -o c_examples/${program_name}.ll
# clang++ -std=c++17 -fpass-plugin=./build/libllvm_instrumentation_pass.so c_examples/${program_name}.cpp ${logger} ${optimization} -o c_examples/${program_name}.out

# clang++ -std=c++17 -fpass-plugin=./build/libllvm_visualizer_pass.so c_examples/${program_name}.cpp ${logger} ${optimization} -emit-llvm -S


clang++ -std=c++20 -fpass-plugin=./build/libllvm_instrumentation_pass.so \
    ./c_examples/fact.cpp \
    ./build/CMakeFiles/logger_obj.dir/src/logger.cpp.o \
    ./build/libs/serializer/libserializer.a \
    -lprotobuf \
    -labsl_hash -labsl_city -labsl_raw_hash_set \
    -labsl_log_internal_message -labsl_log_internal_check_op \
    -lpthread \
    -I./inc -I./build/libs/serializer/generated \
    -o fact.out

./fact.out 20

protoc --decode=instrumentation.ExecutionData \
       --proto_path=libs/serializer/ \
       libs/serializer/dynamic_info.proto < dynamic_info.bin > dynamic_info.result

protoc --decode=gb_ser.Graph libs/serializer/static_info.proto   < static_info.bin > static_info.result   
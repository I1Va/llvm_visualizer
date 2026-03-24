#!/bin/bash
cd instrumentation_pass

cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Release 
cmake --build build

cd ..

program_name="test"
optimization="-O2"
logger="./instrumentation_pass/build/CMakeFiles/logger_obj.dir/src/logger.cpp.o"
serializer="./instrumentation_pass/build/serializer/libserializer.a"
plugin="./instrumentation_pass/build/libllvm_instrumentation_pass.so"

clang++ -std=c++20 -fpass-plugin="${plugin}" \
    ./c_examples/${program_name}.cpp \
    ${logger} \
    ${serializer} \
    -lprotobuf \
    -labsl_hash -labsl_city -labsl_raw_hash_set \
    -labsl_log_internal_message -labsl_log_internal_check_op \
    -lpthread \
    -I./inc -I./build/libs/serializer/generated \
    ${optimization} \
    -o fact.out

clang++ -std=c++20 -fpass-plugin="${plugin}" \
    ./c_examples/${program_name}.cpp -emit-llvm ${optimization} -S -o ./c_examples/${program_name}.ll

./fact.out 20

protoc --decode=instrumentation.ExecutionData \
       --proto_path=libs/serializer/ \
       libs/serializer/dynamic_info.proto < info/dynamic_info.bin > info/dynamic_info.result

protoc --decode=gb_ser.Graph libs/serializer/static_info.proto < info/static_info.bin > info/static_info.result   

cd dot_builder
./release.sh
cd ..

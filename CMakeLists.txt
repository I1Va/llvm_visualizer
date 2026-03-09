cmake_minimum_required(VERSION 3.10)
project(llvm_visualizer_pass)
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED
    main.cpp
)

execute_process(
    COMMAND llvm-config --includedir
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${LLVM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
)
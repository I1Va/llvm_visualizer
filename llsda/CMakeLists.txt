cmake_minimum_required(VERSION 3.10)
project(llvm_sda_pass)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/graph_builder ${CMAKE_CURRENT_BINARY_DIR}/graph_builder)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/serializer    ${CMAKE_CURRENT_BINARY_DIR}/serializer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/dynamic_info  ${CMAKE_CURRENT_BINARY_DIR}/dynamic_info)

# ---------- Instrumentation llvm pass ----------
add_library(${PROJECT_NAME} SHARED
    main.cpp
)

execute_process(
    COMMAND llvm-config --includedir
    OUTPUT_VARIABLE LLVM_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    graph_builder
    serializer
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${LLVM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
)

# ---------- Logger object utility file ----------
add_library(logger_obj OBJECT 
    src/logger.cpp
)

target_link_libraries(logger_obj PRIVATE
    serializer
)

target_include_directories(logger_obj
    PRIVATE
    ${LLVM_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/inc
)

target_link_libraries(logger_obj PRIVATE 
    serializer
    dynamic_info
)
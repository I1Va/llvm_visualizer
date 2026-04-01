cmake_minimum_required(VERSION 3.20)
project(serializer LANGUAGES CXX)

set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

add_library(${PROJECT_NAME} 
  ${CMAKE_CURRENT_SOURCE_DIR}/static_info.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_info.proto
  ${CMAKE_CURRENT_SOURCE_DIR}/src/dyn_info_serializer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/graph_serializer.cpp
  ${PROTO_SRCS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${PROTO_GEN_DIR}
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
    protobuf::libprotobuf
    graph_builder
    dynamic_info
    absl::base
    absl::strings
    absl::synchronization
    absl::log_internal_check_op
    absl::log_internal_message
    absl::log_internal_nullguard
    absl::log_initialize
    absl::raw_logging_internal
)

protobuf_generate(
  TARGET ${PROJECT_NAME}
  LANGUAGE cpp
  PROTOC_OUT_DIR "${PROTO_GEN_DIR}"
  IMPORT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
)
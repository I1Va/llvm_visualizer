cmake_minimum_required(VERSION 3.20)
project(serializer LANGUAGES CXX)

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/graph.proto)
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

if(DEFINED Protobuf_PROTOC_EXECUTABLE AND NOT EXISTS "${Protobuf_PROTOC_EXECUTABLE}")
  message(FATAL_ERROR "Protobuf_PROTOC_EXECUTABLE is set but does not exist: ${Protobuf_PROTOC_EXECUTABLE}")
endif()

set(PROTO_GEN_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")

file(MAKE_DIRECTORY "${PROTO_GEN_DIR}")

target_include_directories(${PROJECT_NAME}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../graph_builder/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${PROTO_GEN_DIR}
)

target_link_libraries(serializer 
    PRIVATE 
    protobuf::libprotobuf
    absl::log_internal_check_op
    absl::log_internal_message
)

protobuf_generate(
  TARGET ${PROJECT_NAME}
  LANGUAGE cpp
  PROTOC_OUT_DIR "${PROTO_GEN_DIR}"
  IMPORT_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
  PROTOS
    "${CMAKE_CURRENT_SOURCE_DIR}/graph.proto"
)

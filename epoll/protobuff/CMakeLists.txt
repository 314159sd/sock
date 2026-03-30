cmake_minimum_required(VERSION 3.22)
project(protobuff)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_library(data SHARED ${SRC})
# target_link_libraries(data protobuff)
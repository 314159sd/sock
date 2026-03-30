cmake_minimum_required(VERSION 3.22)
project(log)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)

add_library(log SHARED ${SRC})
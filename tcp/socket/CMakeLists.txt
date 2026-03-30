cmake_minimum_required(VERSION 3.22)
project (net)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
message(STATUS ${SRC})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

add_library(socket SHARED ${SRC})
target_link_libraries(socket log)
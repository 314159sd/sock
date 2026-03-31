cmake_minimum_required(VERSION 3.22)
project(tcp)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/head/*.cpp)

add_library(data SHARED ${SRC})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/head)

add_executable(server server.cpp)
target_link_libraries(server data)
add_executable(client client.cpp)
target_link_libraries(client data)

# add_executable(main main.cpp)
# target_link_libraries(main data)


cmake_minimum_required(VERSION 3.9.0)
project(tcp VERSION 0.1.0 LANGUAGES C CXX)

set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_executable(server server.cpp)
add_executable(client client.cpp)
target_link_libraries(server socket )
target_link_libraries(client socket log)

# add_subdirectory(Logger)
add_subdirectory(socket)
add_subdirectory(log)
cmake_minimum_required(VERSION 3.22)
project(tcp)

add_subdirectory(protobuff)


LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_executable(server server.cpp)
add_executable(client client.cpp)
target_link_libraries(server data)
target_link_libraries(client data)

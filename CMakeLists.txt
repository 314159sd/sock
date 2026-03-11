cmake_minimum_required(VERSION 3.22)
project(tcp)

add_executable(server server.cpp)

add_executable(client client.cpp)


add_executable(t_server t_server.cpp)

add_executable(t_client t_client.cpp)
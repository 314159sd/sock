cmake_minimum_required(VERSION 3.9.0)
project(tcp VERSION 0.1.0 LANGUAGES C CXX)

add_executable(server server.cpp)
add_executable(server1 server.c)
add_executable(client client.cpp)
add_executable(client1 client.c)
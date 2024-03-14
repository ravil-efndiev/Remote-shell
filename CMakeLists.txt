cmake_minimum_required(VERSION 3.7)

project(crpg)

set(CMAKE_CXX_STANDARD 20)

add_executable(
    server 
    src/server.cpp 
    src/shell.cpp 
    src/core.cpp
)
add_executable(
    client 
    src/client.cpp 
    src/core.cpp
)

set(CMAKE_CXX_FLAGS "-O3 -Wall")

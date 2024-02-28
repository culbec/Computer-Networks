cmake_minimum_required(VERSION 3.26)
project(lab1 C)

set(CMAKE_C_STANDARD 23)

add_executable(lab1
        client.c
        server.c)

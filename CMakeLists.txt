cmake_minimum_required(VERSION 3.10)

project(mips C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(src)
set(SOURCES src/main.c test.c)

add_executable(mips ${SOURCES})
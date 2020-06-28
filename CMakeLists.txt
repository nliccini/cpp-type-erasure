cmake_minimum_required(VERSION 3.10)
project(type-erasure)

set(CMAKE_CXX_STANDARD 17)

add_executable(type-erasure-test test/main.cpp)
target_include_directories(type-erasure-test PUBLIC inc)
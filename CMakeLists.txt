cmake_minimum_required(VERSION 3.14)
project(AoC22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

include(dependencies.cmake)

add_subdirectory(src)

cmake_minimum_required(VERSION 2.8)

enable_language(Fortran)

set(CMAKE_Fortran_FLAGS "-O3")

file(GLOB SRC "*.f")

add_library(spmm STATIC ${SRC})
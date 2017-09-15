cmake_minimum_required(VERSION 2.8)

project(spmm Fortran)

set(CMAKE_Fortran_FLAGS "-O3")

file(GLOB SRC "*.f")

add_library(${PROJECT_NAME} STATIC ${SRC})
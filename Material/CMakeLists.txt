cmake_minimum_required(VERSION 3.8)

project(Material)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/Configuration.cmake)

include(1D/CMakeLists.txt)
include(2D/CMakeLists.txt)
include(3D/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
	Material.cpp MaterialTemplate.cpp MaterialParser.cpp
	${M1D} ${M2D} ${M3D})
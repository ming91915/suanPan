cmake_minimum_required(VERSION 3.0)

project(MaterialExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/Configuration.cmake)

add_library(${PROJECT_NAME} SHARED
	"MaterialExample.cpp"
	"${ROOT}/Material/Material.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Toolbox/debug.cpp")

target_link_libraries(${PROJECT_NAME} arpack superlu)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} openblas gfortran quadmath)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME} lapack)
endif()
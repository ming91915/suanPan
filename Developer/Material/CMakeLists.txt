cmake_minimum_required(VERSION 3.0)

project(MaterialExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/config.cmake)

add_library(${PROJECT_NAME} SHARED
    "MaterialExample.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Material/Material.cpp"
    "${ROOT}/Toolbox/debug.cpp"
    )

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

target_link_libraries(${PROJECT_NAME} arpack superlu)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} openblas gfortran quadmath)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME} lapack)
endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} openblas gfortran quadmath)
endif()

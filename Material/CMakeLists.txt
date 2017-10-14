cmake_minimum_required(VERSION 3.0)

project(Material)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/.config.cmake)

include(Material1D/CMakeLists.txt)
include(Material2D/CMakeLists.txt)
include(Material3D/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
    Material.cpp
    MaterialTemplate.cpp
    MaterialParser.cpp
    ${M1D}
    ${M2D}
    ${M3D}
    )
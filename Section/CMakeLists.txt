cmake_minimum_required(VERSION 3.0)

project(Section)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/.config.cmake)

include(Section1D/CMakeLists.txt)
include(Section2D/CMakeLists.txt)
include(Section3D/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
    ParameterType.cpp
    Section.cpp
    SectionParser.cpp
    ${S1D} ${S2D} ${S3D}
    )
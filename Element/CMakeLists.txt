cmake_minimum_required(VERSION 3.0)

project(Element)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/config.cmake)

include(Cube/CMakeLists.txt)
include(Membrane/CMakeLists.txt)
include(Section/CMakeLists.txt)
include(Truss/CMakeLists.txt)
include(Beam/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
    Element.cpp ElementTemplate.cpp ElementParser.cpp
    ${Cube} ${Membrane} ${Section} ${Truss})
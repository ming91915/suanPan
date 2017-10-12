cmake_minimum_required(VERSION 3.0)

project(Section)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/.config.cmake)

add_library(${PROJECT_NAME} STATIC
    Section.cpp
    SectionParser.cpp
    Rectangle.cpp
    )
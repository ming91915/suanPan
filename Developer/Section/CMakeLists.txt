cmake_minimum_required(VERSION 3.0)

project(SectionExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/.config.cmake)

add_library(${PROJECT_NAME} SHARED
    "SectionExample.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Material/Material.cpp"
    "${ROOT}/Section/Section.cpp"
    "${ROOT}/Toolbox/debug.cpp"
    )

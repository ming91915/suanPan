cmake_minimum_required(VERSION 3.0)

project(ElementExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/config.cmake)

add_library(${PROJECT_NAME} SHARED
    "ElementExample.cpp"
    "${ROOT}/Element/Element.cpp"
    "${ROOT}/Domain/DomainBase.cpp"
    "${ROOT}/Domain/Node.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Toolbox/debug.cpp"
    )

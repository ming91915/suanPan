cmake_minimum_required(VERSION 3.0)

project(ElementExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/config.cmake)

add_library(${PROJECT_NAME} SHARED
    "ElementExample.cpp"
    "${ROOT}/Constraint/Constraint.cpp"
    "${ROOT}/Domain/Domain.cpp"
    "${ROOT}/Domain/Node.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Domain/Workshop.cpp"
    "${ROOT}/Element/Element.cpp"
    "${ROOT}/Load/Amplitude/Amplitude.cpp"
    "${ROOT}/Load/Load.cpp"
    "${ROOT}/Toolbox/RCM.cpp"
    "${ROOT}/Toolbox/debug.cpp"
    )

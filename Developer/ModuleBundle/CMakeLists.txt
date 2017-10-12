cmake_minimum_required(VERSION 3.0)

project(ModuleBundle)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/.config.cmake)
include_directories(../Element)
include_directories(../Material)

add_library(${PROJECT_NAME} SHARED
    "${ROOT}/Domain/DomainBase.cpp"
    "${ROOT}/Domain/Node.cpp"
    "${ROOT}/Domain/Tag.cpp"
    "${ROOT}/Element/Element.cpp"
    "${ROOT}/Material/Material.cpp"
    "${ROOT}/Section/Section.cpp"
    "${ROOT}/Toolbox/debug.cpp"
    "../Element/ElementExample.cpp"
    "../Material/MaterialExample.cpp"
    "../Section/SectionExample.cpp"
    )

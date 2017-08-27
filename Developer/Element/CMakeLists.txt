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
    "${ROOT}/Domain/Workroom.cpp"
    "${ROOT}/Element/Element.cpp"
    "${ROOT}/Load/Amplitude/Amplitude.cpp"
    "${ROOT}/Load/Load.cpp"
    "${ROOT}/Toolbox/RCM.cpp"
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

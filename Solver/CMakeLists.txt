cmake_minimum_required(VERSION 3.0)

project(Solver)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/.config.cmake)

include(ODE_Solver/CMakeLists.txt)
include(Integrator/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
    Arnoldi.cpp
    BFGS.cpp
    MPDC.cpp
    Newton.cpp
    Ramm.cpp
    Solver.cpp
    ${ODE_Solver}
    ${Integrator}
    )
cmake_minimum_required(VERSION 3.0)

project(Converger)

set(ROOT ..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../Libs)

include(${ROOT}/.config.cmake)

add_library(${PROJECT_NAME} STATIC
        Converger.cpp
        AbsDisp.cpp
        AbsError.cpp
        AbsIncreDisp.cpp
        AbsIncreEnergy.cpp
        AbsResidual.cpp
        RelDisp.cpp
        RelError.cpp
        RelIncreDisp.cpp
        RelIncreEnergy.cpp
        RelResidual.cpp
        )
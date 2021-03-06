cmake_minimum_required(VERSION 3.0)

project(suanPan)

set(ROOT .)

include(${ROOT}/.config.cmake)

link_directories(${PROJECT_BINARY_DIR}/Libs)

add_executable(${PROJECT_NAME}
        "suanPan.h"
        "suanPan_Main.cpp"
        "Constraint/CMakeLists.txt"
        "Domain/CMakeLists.txt"
        "Load/CMakeLists.txt"
        "Recorder/CMakeLists.txt"
        "Step/CMakeLists.txt"
        "Toolbox/CMakeLists.txt")

add_subdirectory(Converger)
add_subdirectory(Element)
add_subdirectory(Material)
add_subdirectory(Section)
add_subdirectory(Solver)

add_subdirectory(Constraint)
add_subdirectory(Domain)
add_subdirectory(Load)
add_subdirectory(Recorder)
add_subdirectory(Step)
add_subdirectory(Toolbox)

if(BUILD_DLL_EXAMPLE)
    add_subdirectory(Developer/Element)
    add_subdirectory(Developer/Material)
    add_subdirectory(Developer/Section)
    add_subdirectory(Developer/ModuleBundle)
endif()

target_link_libraries(${PROJECT_NAME} Converger Element Material Section Solver)

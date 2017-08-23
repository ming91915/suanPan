cmake_minimum_required(VERSION 3.0)

project(suanPan)

set(ROOT .)

include(${ROOT}/Configuration.cmake)

link_directories(${PROJECT_BINARY_DIR}/Libs)

add_executable(${PROJECT_NAME}
        "suanPan.h"
        "suanPan_Main.cpp"
        "Constraint/CMakeLists.txt"
        "Convergence/CMakeLists.txt"
        "Domain/CMakeLists.txt"
        "Load/CMakeLists.txt"
        "Recorder/CMakeLists.txt"
        "Solver/CMakeLists.txt"
        "Step/CMakeLists.txt"
        "Toolbox/CMakeLists.txt")

add_subdirectory(Element)
add_subdirectory(Material)

add_subdirectory(Constraint)
add_subdirectory(Convergence)
add_subdirectory(Domain)
add_subdirectory(Load)
add_subdirectory(Recorder)
add_subdirectory(Solver)
add_subdirectory(Step)
add_subdirectory(Toolbox)

if(BUILD_DLL_EXAMPLE)
	add_subdirectory(Developer/Element)
    add_subdirectory(Developer/Material)
endif()

target_link_libraries(${PROJECT_NAME} Element Material)

target_link_libraries(${PROJECT_NAME} arpack superlu)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

        if(USE_OPENBLAS)
            target_link_libraries(${PROJECT_NAME} openblas)
        elseif(USE_NETLIB)
            target_link_libraries(${PROJECT_NAME} openblas)
        else()
            message("Please check either USE_OPENBLAS or USE_NETLIB.")
        endif()

        target_link_libraries(${PROJECT_NAME} gfortran quadmath)

        if(USE_HDF5)
            target_link_libraries(${PROJECT_NAME} hdf5_cpp-static hdf5-static)
        endif()

    endif()
    
    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

        target_link_libraries(${PROJECT_NAME} lapack)

        if(USE_HDF5)
        target_link_libraries(${PROJECT_NAME} libhdf5_cpp libhdf5)
        endif()

    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} openblas gfortran quadmath)
endif()

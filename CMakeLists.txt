cmake_minimum_required(VERSION 3.8)

project(suanPan)

include_directories(.)
include_directories(./Include/armadillo)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
option(USE_NETLIB "USE NETLIB LAPCK" OFF)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

option(USE_HDF5 "USE HDF5 LIBRARY" ON)
option(USE_OPENBLAS "USE OPENBLAS" ON)
option(USE_OPENMP "USE OPENMP" OFF)

if (USE_HDF5)
    include_directories(./Include/hdf5-common)
endif ()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (USE_HDF5)
        include_directories(./Include/hdf5-gcc)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif ()
    link_directories(./Libs/gcc)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
    #if (USE_OPENMP)
    #    set(CMAKE_CXX_FLAGS "-fopenmp")
    #    set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
    #    add_definitions(-DSUANPAN_OPENMP)
    #endif ()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (USE_HDF5)
        include_directories(./Include/hdf5-msvc)
    else()
        add_definitions(/D ARMA_DONT_USE_HDF5)
    endif ()
    link_directories(./Libs/msvc)
    set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
    set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
endif ()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions(-DARMA_DONT_USE_HDF5)
    add_definitions(-DARMA_DONT_USE_ARPACK)
    add_definitions(-DARMA_DONT_USE_SUPERLU)
    link_directories(./Libs/linux)
    link_libraries("-ldl -pthread")
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
    set(CMAKE_CXX_FLAGS "-fexceptions")

endif ()

add_executable(${PROJECT_NAME}
        "suanPan.h"
        "suanPan_Main.cpp"
        "Constraint/CMakeLists.txt"
        "Convergence/CMakeLists.txt"
        "Domain/CMakeLists.txt"
        "Element/CMakeLists.txt"
        "Load/CMakeLists.txt"
        "Material/CMakeLists.txt"
        "Recorder/CMakeLists.txt"
        "Solver/CMakeLists.txt"
        "Step/CMakeLists.txt"
        "Toolbox/CMakeLists.txt")

add_subdirectory(Constraint)
add_subdirectory(Convergence)
add_subdirectory(Domain)
add_subdirectory(Element)
add_subdirectory(Load)
add_subdirectory(Material)
add_subdirectory(Recorder)
add_subdirectory(Solver)
add_subdirectory(Step)
add_subdirectory(Toolbox)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME}
            libarpack.a
            libsuperlu.a)
    if (USE_OPENBLAS)
        target_link_libraries(${PROJECT_NAME} libopenblas.a)
    elseif(USE_NETLIB)
        target_link_libraries(${PROJECT_NAME} libopenblas.a)
    else()
        message("Please check either USE_OPENBLAS or USE_NETLIB.")
    endif ()
    target_link_libraries(${PROJECT_NAME}
            libgfortran.a
            libquadmath.a)
    if (USE_HDF5)
    target_link_libraries(${PROJECT_NAME}
            libhdf5_cpp-static.a
            libhdf5-static.a)
    endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME}
            arpack.lib
            superlu.lib
            lapack.lib)
    if (USE_HDF5)
    target_link_libraries(${PROJECT_NAME}
            libhdf5_cpp.lib
            libhdf5.lib)
    endif()
endif ()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME}
    	    libopenblas.a
            libgfortran.a
            libquadmath.a)
endif ()


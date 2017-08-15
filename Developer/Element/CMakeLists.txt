cmake_minimum_required(VERSION 3.8)

project(ElementExample)

include_directories(.)
include_directories(../../)
include_directories(../../Include/armadillo)

add_definitions(-DARMA_DONT_USE_HDF5)
add_definitions(-DSUANPAN_DLL)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    link_directories(../../Libs/gcc)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    link_directories(../../Libs/msvc)
    set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
    set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
endif ()

add_library(${PROJECT_NAME} SHARED
        "ElementExample.cpp"
        "../../Element/Element.cpp"
        "../../Load/Amplitude/Amplitude.cpp"
        "../../Load/Load.cpp"
        "../../Load/RCM.cpp"
        "../../Domain/Tag.cpp"
        "../../Domain/Domain.cpp"
        "../../Domain/Node.cpp"
        "../../Domain/Workroom.cpp"
        "../../Constraint/Constraint.cpp"
        "../../Toolbox/debug.cpp")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME}
            libarpack.a
            libsuperlu.a
            libopenblas.a
            libgfortran.a
            libquadmath.a)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME} arpack.lib superlu.lib lapack.lib)
endif ()
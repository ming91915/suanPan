cmake_minimum_required(VERSION 3.8)

project(suanPan)

include_directories(.)
include_directories(./Include/armadillo)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include_directories(./Include/hdf5-gcc)
    link_directories(./Libs/gcc)
    set(CMAKE_CXX_STANDARD 14)
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
    set(CMAKE_CXX_FLAGS "-fexceptions")
    # set(CMAKE_CXX_FLAGS "-fopenmp")
    # set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    include_directories(./Include/hdf5-msvc)
    link_directories(./Libs/msvc)
    set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
    set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME}
            libarpack.a
            libsuperlu.a
            libopenblas.a
            libgfortran.a
            libquadmath.a
            libhdf5-static.a
            libhdf5_cpp-static.a)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME}
            arpack.lib
            superlu.lib
            lapack.lib
            libhdf5.lib
            libhdf5_cpp.lib)
endif ()
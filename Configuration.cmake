include_directories(${ROOT})
include_directories(${ROOT}/Include/armadillo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    option(USE_NETLIB "USE NETLIB LAPCK" OFF)
endif()

if (CMAKE_SYSTEM_NAME MATCHES "Windows")

    option(USE_HDF5 "USE HDF5 LIBRARY" ON)
    option(USE_OPENBLAS "USE OPENBLAS" ON)
    option(USE_OPENMP "USE OPENMP" OFF)

    if (USE_HDF5)
        include_directories(${ROOT}/Include/hdf5-common)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif ()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        if (USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-gcc)
        endif ()
        link_directories(${ROOT}/Libs/gcc)

        set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
        set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
        #if (USE_OPENMP)
        #    set(CMAKE_CXX_FLAGS "-fopenmp")
        #    set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
        #    add_definitions(-DSUANPAN_OPENMP)
        #endif ()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        if (USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-msvc)
        endif ()
        link_directories(${ROOT}/Libs/msvc)
        set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
        set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
    endif ()

elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")

    add_definitions(-DARMA_DONT_USE_HDF5)
    add_definitions(-DARMA_DONT_USE_ARPACK)
    add_definitions(-DARMA_DONT_USE_SUPERLU)
    link_directories(${ROOT}/Libs/linux)
    link_libraries("-ldl -pthread")

    set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")

endif ()
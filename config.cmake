include_directories(${ROOT})
include_directories(${ROOT}/Include/armadillo)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)

option(BUILD_DLL_EXAMPLE "BUILD DYNAMIC LIBRARY EXAMPLE" ON)
option(USE_HDF5 "USE HDF5 LIBRARY" ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    option(USE_NETLIB "USE NETLIB LAPCK" OFF)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

    link_libraries(arpack superlu)

    option(USE_OPENBLAS "USE OPENBLAS" ON)
    option(USE_OPENMP "USE OPENMP" OFF)

    if(USE_HDF5)
        include_directories(${ROOT}/Include/hdf5-common)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")

        if(USE_OPENBLAS)
            link_libraries(openblas)
        elseif(USE_NETLIB)
            link_libraries(openblas)
        else()
            message("Please check either USE_OPENBLAS or USE_NETLIB.")
        endif()

        link_libraries(gfortran quadmath)

        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-gcc)
        	link_libraries(hdf5_cpp-static hdf5-static)
        endif()
        link_directories(${ROOT}/Libs/gcc)

        set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
        set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
        #if(USE_OPENMP)
        #    set(CMAKE_CXX_FLAGS "-fopenmp")
        #    set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
        #    add_definitions(-DSUANPAN_OPENMP)
        #endif ()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        link_libraries(lapack)
        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-msvc)
        	link_libraries(libhdf5_cpp libhdf5)
        endif()
        link_directories(${ROOT}/Libs/msvc)
        set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
        set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    link_directories(${ROOT}/Libs/linux)
    link_libraries(arpack superlu openblas gfortran quadmath dl)
    
    if(USE_HDF5)
        set(HDF5_PATH /usr/local/HDF5 CACHE PATH "Please make sure the HDF5 library is installed under /usr/local/HDF5 or specify another location.")
        include_directories(${HDF5_PATH}/include)
        link_directories(${HDF5_PATH}/lib)
        link_libraries(hdf5_cpp-static hdf5-static)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif()

    set(CMAKE_CXX_FLAGS "-O3 -fexceptions -fopenmp")
    set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
endif()

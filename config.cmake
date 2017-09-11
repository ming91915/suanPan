include_directories(${ROOT})
include_directories(${ROOT}/Include/armadillo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(USE_HDF5 "USE HDF5 LIBRARY" OFF)
option(USE_NETLIB "USE NETLIB BLAS AND LAPACK" ON)
option(USE_OPENMP "USE OPENMP LIBRARY" OFF)
option(USE_OPENBLAS "USE OPENBLAS LIBRARY" OFF)
option(BUILD_DLL_EXAMPLE "BUILD DYNAMIC LIBRARY EXAMPLE" ON)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")

    if(USE_HDF5)
        include_directories(${ROOT}/Include/hdf5-common)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        link_directories(${ROOT}/Libs/gcc)
        link_libraries(arpack superlu spmm)
        if(USE_OPENBLAS)
            link_libraries(openblas)
        elseif(USE_NETLIB)
            link_libraries(lapack)
        endif()
        link_libraries(gfortran quadmath)
        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-gcc)
        	link_libraries(hdf5_cpp-static hdf5-static)
        endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        link_directories(${ROOT}/Libs/msvc)
        link_libraries(superlu lapack)
        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-msvc)
        	link_libraries(libhdf5_cpp libhdf5)
        endif()
        set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
        set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")
    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")

    link_directories(${ROOT}/Libs/linux)
    link_libraries(arpack superlu spmm)
    if(USE_OPENBLAS)
        link_libraries(openblas)
    elseif(USE_NETLIB)
        link_libraries(lapack)
    endif()
    link_libraries(gfortran quadmath dl)
    
    if(USE_HDF5)
        set(HDF5_PATH /usr/local/HDF5 CACHE PATH "Please make sure the HDF5 library is installed under /usr/local/HDF5 or specify another location.")
        include_directories(${HDF5_PATH}/include)
        link_directories(${HDF5_PATH}/lib)
        link_libraries(hdf5_cpp-static hdf5-static)
    else()
        add_definitions(-DARMA_DONT_USE_HDF5)
    endif()

endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if(USE_OPENMP)
        set(CMAKE_CXX_FLAGS "-fopenmp")
        set(CMAKE_CXX_FLAGS_DEBUG "-fopenmp")
        set(CMAKE_EXE_LINKER_FLAGS "-fopenmp")
        add_definitions(-DSUANPAN_OPENMP)
    endif ()
    set(CMAKE_CXX_FLAGS "-O3 -fexceptions")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
endif()

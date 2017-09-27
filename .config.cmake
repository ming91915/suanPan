include_directories(${ROOT})
include_directories(${ROOT}/Include/armadillo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(USE_HDF5 "USE HDF5 LIBRARY" ON)
option(USE_NETLIB "USE NETLIB BLAS AND LAPACK" ON)
option(USE_OPENBLAS "USE OPENBLAS LIBRARY" OFF)
option(BUILD_MULTI_THREAD "BUILD MULTI THREAD VERSION" OFF)
option(BUILD_DLL_EXAMPLE "BUILD DYNAMIC LIBRARY EXAMPLE" ON)
option(TEST_COVERAGE "TEST CODE COVERAGE USING GCOV" OFF)

if(USE_HDF5)
    include_directories(${ROOT}/Include/hdf5-common)
else()
    add_definitions(-DSUANPAN_NO_HDF5)
    add_definitions(-DARMA_DONT_USE_HDF5)
endif()

if(BUILD_MULTI_THREAD)
    add_definitions(-DSUANPAN_MT)
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Windows") # WINDOWS PLATFORM

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU") # GNU GCC COMPILER

        link_directories(${ROOT}/Libs/gcc)

        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-gcc)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC") # MSVC COMPILER

        link_directories(${ROOT}/Libs/msvc)

        link_libraries(superlu)
        if((USE_OPENBLAS) AND (NOT USE_NETLIB))
            link_libraries(arpack spmm libopenblas)
        elseif((USE_NETLIB) AND (NOT USE_OPENBLAS))
            link_libraries(lapack)
        else()
            message(FATAL_ERROR "Pease check either USE_NETLIB or USE_OPENBLAS.")
        endif()

        if(USE_HDF5)
            include_directories(${ROOT}/Include/hdf5-msvc)
        endif()
        
        set(CMAKE_CXX_FLAGS "/MP /EHsc /arch:AVX")
        set(CMAKE_CXX_FLAGS_DEBUG "/D \"DEBUG\"")

    endif()

    if(USE_HDF5)
        link_libraries(libhdf5_hl libhdf5 libszip libzlib)
    endif()

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux") # LINUX PLATFORM

    link_directories(${ROOT}/Libs/linux) # GNU GCC COMPILER
    link_libraries(dl)

    if(USE_HDF5)
        include_directories(${ROOT}/Include/hdf5-linux)
        link_libraries(hdf5_hl hdf5 szip z)
    endif()

endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU") # GNU GCC COMPILER

    link_libraries(arpack superlu spmm)
    if((USE_OPENBLAS) AND (NOT USE_NETLIB))
        link_libraries(openblas pthread)
    elseif((USE_NETLIB) AND (NOT USE_OPENBLAS))
        link_libraries(lapack)
    else()
        message(FATAL_ERROR "Pease check either USE_NETLIB or USE_OPENBLAS.")
    endif()
    link_libraries(gfortran quadmath)

    set(CMAKE_EXE_LINKER_FLAGS "-fPIC")

    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -fexceptions -mavx")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG")
    if(TEST_COVERAGE) # COVERAGE ONLY AVAILABLE UNDER GCC
        set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
        link_libraries(gcov)
    endif()

endif()

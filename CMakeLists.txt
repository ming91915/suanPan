cmake_minimum_required(VERSION 3.8)

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

target_link_libraries(${PROJECT_NAME} Element Material)

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

cmake_minimum_required(VERSION 3.0)

project(ElementExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../..)

include(${ROOT}/Configuration.cmake)

add_library(${PROJECT_NAME} SHARED
	"ElementExample.cpp"
	"${ROOT}/Constraint/Constraint.cpp"
	"${ROOT}/Domain/Domain.cpp"
	"${ROOT}/Domain/Node.cpp"
	"${ROOT}/Domain/Tag.cpp"
	"${ROOT}/Domain/Workroom.cpp"
	"${ROOT}/Element/Element.cpp"
	"${ROOT}/Load/Amplitude/Amplitude.cpp"
	"${ROOT}/Load/Load.cpp"
	"${ROOT}/Load/RCM.cpp"
	"${ROOT}/Toolbox/debug.cpp")

target_link_libraries(${PROJECT_NAME} arpack superlu)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} openblas gfortran quadmath)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_link_libraries(${PROJECT_NAME} lapack)
endif()
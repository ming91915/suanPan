cmake_minimum_required(VERSION 3.8)

project(ElementExample)

add_definitions(-DSUANPAN_DLL)

set(ROOT ../..)

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
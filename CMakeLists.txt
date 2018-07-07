cmake_minimum_required (VERSION 3.0)

project (csygit)
aux_source_directory(. DIR_SRCS)
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")  
	add_compile_options(-O3) 
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable (
	csygit
	${DIR_SRCS}
)

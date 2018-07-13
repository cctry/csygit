cmake_minimum_required (VERSION 3.4)

project (csygit)
aux_source_directory(. DIR_SRCS)
if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++14)
    message(STATUS "optional:-std=c++14")  
	add_compile_options(-O3) 
	add_compile_options(-Wall)
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable (
	csygit
	${DIR_SRCS}
)

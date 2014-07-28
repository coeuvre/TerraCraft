cmake_minimum_required(VERSION 2.8)

macro(separate_with_space _varname)
    set(_list "${${_varname}}")
    set(${_varname} "")
    foreach(_var ${_list})
        set(${_varname} "${${_varname}} ${_var}")
    endforeach(_var)
endmacro()

project(TerraCraft)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

add_subdirectory(src)


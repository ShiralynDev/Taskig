cmake_minimum_required(VERSION 3.20)
project(Taskig)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_SHARED_LIBS OFF)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_SHARED_LIBS ON)
endif()

set(BUILD_EXAMPLES OFF)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_executable(Taskig ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(Taskig PRIVATE -O3)
    
    if (NOT WIN32)
        target_link_options(Taskig PRIVATE -s)
    endif()
endif()
cmake_minimum_required(VERSION 3.29)

project(external-eastl)
include(ExternalProject)

set(_cxx_flags "${CMAKE_CXX_FLAGS} -DEASTL_USER_DEFINED_ALLOCATOR")

list(APPEND CMAKE_ARGS
    "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DCMAKE_CXX_FLAGS=${_cxx_flags}"
    
    )

message(STATUS "Preparing external project \"eastl\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

# do shallow copy without recursive update because of long path error for git_dir, so we update submodules in update step

ExternalProject_add(
    eastl
    GIT_REPOSITORY https://github.com/electronicarts/EASTL
    GIT_TAG 3.21.12
    GIT_SUBMODULES_RECURSE 0
    UPDATE_COMMAND git --work-tree=<SOURCE_DIR> submodule update --init
    GIT_SHALLOW 1
    PREFIX eastl
    CMAKE_ARGS "${CMAKE_ARGS}"
    )
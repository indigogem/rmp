cmake_minimum_required(VERSION 3.29)

project(external-bgfx)
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    )

message(STATUS "Preparing external project \"bgfx\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_add(
    BGFX
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake
    GIT_TAG v1.127.8725-469
    GIT_SHALLOW 1
    PREFIX bgfx
    CMAKE_ARGS "${CMAKE_ARGS}"
    )
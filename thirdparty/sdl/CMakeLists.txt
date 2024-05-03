cmake_minimum_required(VERSION 3.29)

project(external-sdl)
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DSDL_SHARED=ON"
    "-DSDL_STATIC=ON"
    )

message(STATUS "Preparing external project \"sdl\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

ExternalProject_add(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL
    #GIT_TAG prerelease-3.1.2
    GIT_TAG origin/main
    GIT_SHALLOW 1
    PREFIX sdl3
    CMAKE_ARGS "${CMAKE_ARGS}"
    )
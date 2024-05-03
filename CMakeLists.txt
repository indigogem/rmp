cmake_minimum_required(VERSION 3.29)

project(kmp C CXX)

set(_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(_lib_name ${PROJECT_NAME})
set( project_TARGETS
     ${_lib_name}
     standalone
   )

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

add_library(${_lib_name})

target_sources(${_lib_name}
    PRIVATE
        ${_source_dir}/defs.h
        ${_source_dir}/base/application/app.h
        ${_source_dir}/base/application/app.cpp
        ${_source_dir}/base/memory/rpmalloc-1.4.5/rpmalloc.h
        ${_source_dir}/base/memory/rpmalloc-1.4.5/rpmalloc.c
    )
    
target_link_libraries(${_lib_name} PRIVATE SDL3::SDL3-static)
target_include_directories(${_lib_name} PUBLIC "${_source_dir}")


add_executable(standalone "${CMAKE_CURRENT_SOURCE_DIR}/src/standalone/main.cpp")
target_link_libraries(standalone PRIVATE ${_lib_name} SDL3::SDL3-static)


# set common cpp properties
set( project_TARGETS
     ${_lib_name}
     standalone
   )

foreach( _target ${project_TARGETS} )
    target_compile_features(${_target} PRIVATE cxx_std_20)
    set_property(TARGET ${_target} PROPERTY 
                 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endforeach()



# cmake -DCMAKE_INSTALL_PREFIX=thirdparty/install -DCMAKE_PREFIX_PATH=%cd%/thirdparty/install -B thirdparty/build -S thirdparty
# cmake --build thirdparty/build

# cmake -DCMAKE_PREFIX_PATH=%cd%/thirdparty/install -B build -S.
# cmake --build build

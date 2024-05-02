cmake_minimum_required(VERSION 3.29)

project(kmp C CXX)


find_package(SDL3 REQUIRED)

set(_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${_source_dir}/*.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${_source_dir}/*.cpp")


add_library(${PROJECT_NAME} ${headers} ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3-static)
target_include_directories(${PROJECT_NAME} PUBLIC "${_source_dir}")

add_executable(standalone "${CMAKE_CURRENT_SOURCE_DIR}/src/standalone/main.cpp")
target_link_libraries(standalone PRIVATE ${PROJECT_NAME} SDL3::SDL3-static)


# set common cpp properties
set( project_TARGETS
     ${PROJECT_NAME}
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

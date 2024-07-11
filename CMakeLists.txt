cmake_minimum_required(VERSION 3.29)

project(kmp C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

message(STATUS "${CMAKE_MODULE_PATH}")


set(_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(_lib_name ${PROJECT_NAME})
set( project_TARGETS
     ${_lib_name}
     standalone
   )

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)
find_package(EASTL MODULE REQUIRED)
find_package(GLM MODULE REQUIRED)
find_package(BGFX REQUIRED)

# message(STATUS "${EASTL_INCLUDE_DIR}")
# message(STATUS "${CMAKE_PREFIX_PATH}/lib")
# message(STATUS "${EASTL_LIB}")
# message(STATUS "${GLM_INCLUDE_DIR}")


message(STATUS "compiler options \"${_lib_name}\" with args:")
foreach(_opt ${CMAKE_CXX_FLAGS})
    message(STATUS "-- ${_opt}")
endforeach()

add_library(${_lib_name})

target_sources(${_lib_name}
    PRIVATE
        ${_source_dir}/defs.h
        ${_source_dir}/base/application/app.h
        ${_source_dir}/base/application/app.cpp
        ${_source_dir}/base/memory/rpmalloc/rpmalloc.h
        ${_source_dir}/base/memory/rpmalloc/rpmalloc.c
        ${_source_dir}/base/memory/memory.h
        ${_source_dir}/base/memory/memory.cpp
        ${_source_dir}/base/memory/ptrs.h
        ${_source_dir}/base/types/eastl_defs.h
        ${_source_dir}/base/types/eastl_defs.cpp
        ${_source_dir}/base/types/array.h
        ${_source_dir}/base/types/string.h
        ${_source_dir}/base/logging/log.h
        ${_source_dir}/base/logging/log.cpp
        ${_source_dir}/base/time/time.h
        ${_source_dir}/base/time/time.cpp
        ${_source_dir}/base/time/timers.h
        ${_source_dir}/base/time/time_stamp.h
        ${_source_dir}/base/time/time_stamp.cpp      
        ${_source_dir}/base/math/math.h  
        ${_source_dir}/base/input/input_state.h
        ${_source_dir}/base/input/input_states/input_state_mouse.h
        ${_source_dir}/base/input/input_states/input_state_keyboard.h
        ${_source_dir}/base/input/input_system.h
        ${_source_dir}/base/input/input_system.cpp
        ${_source_dir}/base/input/input_device.h
        ${_source_dir}/base/input/input_devices/input_device_keyboard_mouse.h
        ${_source_dir}/base/input/input_devices/input_device_keyboard_mouse.cpp
        ${_source_dir}/base/render/renderer.h
        ${_source_dir}/base/render/renderer.cpp
        
        ${_source_dir}/engine/engine.cpp
        ${_source_dir}/engine/engine.h
        ${_source_dir}/engine/update_context.h
    )
    
target_link_libraries(${_lib_name} PUBLIC SDL3::SDL3-static EASTL::EASTL GLM::GLM bgfx::bgfx bgfx::bimg bgfx::bx)

target_include_directories(${_lib_name} PUBLIC "${_source_dir}")


add_executable(standalone "${CMAKE_CURRENT_SOURCE_DIR}/src/standalone/main.cpp")
target_link_libraries(standalone PUBLIC ${_lib_name})


foreach( _target ${project_TARGETS} )
    target_compile_features(${_target} PRIVATE cxx_std_20)
    set_property(TARGET ${_target} PROPERTY 
                 MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    target_compile_options(${_target} PUBLIC "-DEASTL_USER_DEFINED_ALLOCATOR")
endforeach()


# cmake -DCMAKE_INSTALL_PREFIX=thirdparty/install -DCMAKE_PREFIX_PATH=%cd%/thirdparty/install -B thirdparty/build -S thirdparty
# cmake --build thirdparty/build

# cmake -DCMAKE_PREFIX_PATH=%cd%/thirdparty/install -B build -S.
# cmake --build build

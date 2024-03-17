cmake_minimum_required(VERSION 3.29)
project(rmp VERSION 0.1.0 LANGUAGES C CXX)

add_executable(rmp src/main.cpp)
target_compile_features(rmp PRIVATE cxx_std_20)


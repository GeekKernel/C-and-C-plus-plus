cmake_minimum_required(VERSION 3.20.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CXX_STANDARD 23)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")

project(problem_34)

add_executable(problem_34 main.cpp)
target_compile_options(problem_34
PUBLIC "-std=c++23"
)

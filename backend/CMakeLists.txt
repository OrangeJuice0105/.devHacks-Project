cmake_minimum_required(VERSION 3.20)
project(os_arena_backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(os_arena
    src/main.cpp
    src/simulator/scheduler.cpp
    src/simulator/metrics.cpp
    src/simulator/algorithms/fcfs.cpp
    src/simulator/algorithms/rr.cpp
)

target_include_directories(os_arena PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Windows: avoid min/max macros causing issues
if (WIN32)
    target_compile_definitions(os_arena PRIVATE NOMINMAX)
endif()
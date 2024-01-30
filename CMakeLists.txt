cmake_minimum_required(VERSION 3.15)
project(geometry VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

message(STATUS "C++ Requests CMake Options")
message(STATUS "=======================================================")
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(BUILD_GEOMETRY_TESTS "If enabled we will compile unit tests." OFF)
message(STATUS "=======================================================")

if(NOT BUILD_GEOMETRY_TESTS AND DEFINED ENV{BUILD_GEOMETRY_TESTS})
  set(BUILD_GEOMETRY_TESTS "$ENV{BUILD_GEOMETRY_TESTS}")
endif()

include(CTest)

add_subdirectory(src)

if(BUILD_TESTS OR BUILD_GEOMETRY_TESTS)
    add_subdirectory(tests)
    enable_testing()
endif()

target_include_directories(geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)

project(better_sockets)

add_subdirectory(./test/get_request/)

file(GLOB_RECURSE FILES src/**.cpp)

add_library(BetterSockets STATIC ${FILES})
target_include_directories(BetterSockets PUBLIC ./include/)
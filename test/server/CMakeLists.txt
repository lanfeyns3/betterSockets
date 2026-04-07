cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSION OFF)

project(server)

file(GLOB_RECURSE FILES src/**.cpp)

add_executable(server ${FILES})
target_link_libraries(server PRIVATE BetterSockets)
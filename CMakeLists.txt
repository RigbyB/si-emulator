cmake_minimum_required(VERSION 3.25)
project(si_emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(si_emulator main.cpp)

target_link_libraries(si_emulator ${SDL2_LIBRARIES})
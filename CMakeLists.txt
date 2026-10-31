cmake_minimum_required(VERSION 3.20)

project(simul_atoms LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)

add_executable(simul_atoms main.cpp atoms.cpp)
target_link_libraries(simul_atoms PRIVATE raylib)

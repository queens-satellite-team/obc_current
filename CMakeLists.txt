cmake_minimum_required(VERSION 3.23)
project(obc_current)

set(CMAKE_CXX_STANDARD 11)

add_executable(obc_current main.cpp)

add_subdirectory(drivers)
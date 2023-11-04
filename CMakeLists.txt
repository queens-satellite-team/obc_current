cmake_minimum_required(VERSION 3.23)
project(obc_current)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_subdirectory(drivers)

add_executable(obc_current main.cpp)
target_link_libraries(obc_current rtc)

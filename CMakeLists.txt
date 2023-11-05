cmake_minimum_required(VERSION 3.23)
project(obc_current)

set(CMAKE_CXX_STANDARD 11)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(drivers)

find_library(wiringPi_LIB wiringPi)
target_link_libraries(${PROJECT_NAME} ${wiringPi_LIB} i2c)

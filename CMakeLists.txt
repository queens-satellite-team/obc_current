cmake_minimum_required(VERSION 3.23)
project(obc_current)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_subdirectory(drivers)
set(obc_current_SRCS
    ${drivers_SRCS}
    )

add_executable(obc_current main.cpp drivers/rtc.cpp)
find_library(wiringPi_LIB wiringPi)
target_link_libraries(obc_current ${wiringPi_LIB})
add_definitions(-std=c++0x -l/usr/local/lib/libwringpi.so -lwiringpi -lpthread)


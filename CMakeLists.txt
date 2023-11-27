# cmake -S . -B build
# use option -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON if your editor gives errors for includes
cmake_minimum_required(VERSION 3.22)
project(obc_current)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(drivers)

target_link_libraries(${PROJECT_NAME} i2c)

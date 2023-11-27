add_executable(rtctest rtctest.cpp ${CMAKE_SOURCE_DIR}/drivers/rtc/rtc.cpp)


target_link_libraries(rtctest CppUTest i2c)
target_include_directories(rtctest PUBLIC ${CMAKE_SOURCE_DIR}/drivers/rtc)
add_test(NAME rtctest COMMAND $<TARGET_FILE:rtctest>)

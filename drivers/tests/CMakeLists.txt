add_executable(rtc_test rtc_test.cpp ${CMAKE_SOURCE_DIR}/drivers/rtc/rtc.cpp)

target_link_libraries(rtc_test gtest i2c)
target_include_directories(rtc_test PUBLIC ${CMAKE_SOURCE_DIR}/drivers/rtc)
add_test(NAME rtc_test COMMAND $<TARGET_FILE:rtc_test>)

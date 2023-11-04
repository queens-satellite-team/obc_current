include_directories(.)

#list drivers here
set(drivers
    rtc
    )

#each driver should be added here, and listed above
add_library(rtc SHARED rtc/rtc.cpp)
find_library(wiringPi_LIB wiringPi)
target_link_libraries(rtc ${wiringPi_LIB})

cmake_minimum_required(VERSION 3.10)

project(Zero)

set(CMAKE_CXX_STANDARD 11)
set(RASPBERRY 1)
if(RASPBERRY)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
set(CMAKE_ASM_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_AR arm-linux-gnueabihf-ar)
set(CMAKE_OBJCOPY arm-linux-gnueabihf-objcopy)
set(CMAKE_OBJDUMP arm-linux-gnueabihf-objdump)
set(SIZE arm-linux-gnueabihf-size)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/car/common)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/car/Navio)
file(GLOB_RECURSE SOURCES "src/*" "zero.cpp")



add_executable(Zero ${SOURCES}
        src/car/common/gpio.cpp
        src/car/common/gpio.h
        src/common/log.h
        src/car/Navio/rgbled.cpp
        src/car/Navio/rgbled.h
        src/car/common/led.h src/car/common/internalSensor.h src/car/common/spi.h src/car/common/mpu9250.cpp src/car/common/mpu9250.h)

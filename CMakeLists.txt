cmake_minimum_required(VERSION 3.20)

# Pull in SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Pull in FreeRTOS
include($ENV{FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(app C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()
set(NAME lcd_freertos)
set(NAME_SOURCE
    main.cpp
    src/lcd.c
    src/onboard_temp.c
    src/i2c_scan.c
)

add_executable(${NAME} ${NAME_SOURCE})

target_include_directories(${NAME} PRIVATE 
	${CMAKE_CURRENT_LIST_DIR}/include)
 
# pull in common dependencies
target_link_libraries(${NAME} pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4 hardware_i2c hardware_adc)

pico_enable_stdio_usb(lcd_freertos 1)
pico_enable_stdio_uart(lcd_freertos 0)
# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${NAME})


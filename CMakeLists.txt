cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME FreeRTOS_LED)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C CXX)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    ./Users/Src/beep_bsp.c
    ./Users/Src/lcd_bsp.c
    ./Users/Src/led_bsp.c
    ./Users/Src/my_usart.c
    ./Users/Application/lcd_disp.c
    ./Users/font/fonts.c
    ./Users/Src/usart_debug.cpp
    ./Users/Src/alg_foc.cpp
    ./Users/Src/drv_math.cpp
    ./Users/Src/drv_pwm.c
    ./Users/Src/drv_adc.cpp
    ./Users/Src/alg_pid.cpp
    ./Users/Src/alg_slope.cpp
    ./Users/Src/dvc_encoder.cpp
    ./Users/Src/dvc_motor.cpp
    ./Users/Src/dvc_motorTask.cpp
    ./Users/Src/servo_bus_task.cpp
    ./Users/Src/dvc_sampler.cpp
    ./Users/Src/dvc_servo.cpp
    ./Users/Src/alg_filter.cpp
    ./Users/Src/drv_uart.c
    ./Users/Src/ita_control.cpp

)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ./Users
    ./Users/Application
    ./Users/font
    ./Users/Inc
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

set_source_files_properties(Core/Src/main.c PROPERTIES LANGUAGE CXX)
set_source_files_properties(Core/Src/freertos.c PROPERTIES LANGUAGE CXX)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)


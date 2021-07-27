cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(pico-solar
        main.c
        )

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(pico-solar pico_stdlib hardware_adc hardware_irq)

pico_enable_stdio_usb(pico-solar 0)
pico_enable_stdio_uart(pico-solar 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico-solar)

# add url via pico_set_program_url
pico_set_program_url(pico-solar "https://github.com/jknutson/pico-solar")

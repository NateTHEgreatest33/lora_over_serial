cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(SmartHomeCore C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_subdirectory( lib/messageAPI )
add_subdirectory( lib/LoRa )
add_subdirectory( lib/button )
add_subdirectory( lib/util )
add_subdirectory( lib/console )

pico_sdk_init()

add_executable( SmartHomeCore
  src/main.cpp

  src/serial_lora.cpp
  src/serial_lora.hpp
)

# Debugger Attached
# pico_enable_stdio_usb( SmartHomeCore 0)
# pico_enable_stdio_uart( SmartHomeCore 1)

# # No Debugger, uart over USB
pico_enable_stdio_usb( SmartHomeCore 1)
pico_enable_stdio_uart( SmartHomeCore 0)

target_link_libraries(SmartHomeCore
  pico_stdlib
  hardware_spi
  pico_multicore
  
  # Using Pico W
  pico_cyw43_arch_none

  # libraries
  loraAPI
  messageAPI
  buttonComponent
  utilLib
  consoleAPI

)



#make uf2
pico_add_extra_outputs(SmartHomeCore)
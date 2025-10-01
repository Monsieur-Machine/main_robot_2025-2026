cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(motion_controller C CXX ASM)
set(CMAKE_C_STNDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

pico_sdk_init()

add_executable(motion_controller
  src/main.c
  src/robot.c
  src/motors.c
  src/i2c_slave.c
  src/i2c_buffer.c
)

target_link_libraries(motion_controller
  hardware_i2c
  hardware_pwm
  hardware_uart
  pico_stdlib
)

pico_enable_stdio_usb(motion_controller 1)
pico_enable_stdio_uart(motion_controller 1)

pico_add_extra_outputs(motion_controller)

add_custom_target(Flash
    DEPENDS motion_controller
    COMMAND sudo picotool load -f ${PROJECT_BINARY_DIR}/motion_controller.uf2
)

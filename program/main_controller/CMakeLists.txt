cmake_minimum_required(VERSION 3.13)

# Définir explicitement la carte comme Pico W
set(PICO_BOARD pico_w)
if(NOT DEFINED PICO_BOARD)
    add_definitions(-DPICO_BOARD=${PICO_BOARD})
endif()

include(pico_sdk_import.cmake)

project(main_controller C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(main_controller
  src/main.c
  src/robot.c
  src/i2c/motors.c
  src/i2c/gyro.c
  src/motion_control.c
  src/i2c/i2c_master.c
  src/i2c/mcp23017.c
  src/wifi/wifi_operator.c
  src/wifi/udp_client.c
  src/wifi/udp_payload.c
)

target_include_directories(main_controller PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/wifi/headers/
  ${CMAKE_CURRENT_LIST_DIR}/src/
)

target_link_libraries(main_controller
  pico_stdlib
  hardware_i2c
  hardware_pwm
  hardware_uart
  pico_cyw43_arch_lwip_poll
)

pico_enable_stdio_usb(main_controller 1)
pico_enable_stdio_uart(main_controller 1)

pico_add_extra_outputs(main_controller)

add_custom_target(Flash
    DEPENDS main_controller

    COMMAND sudo picotool load -f ${PROJECT_BINARY_DIR}/main_controller.uf2
)

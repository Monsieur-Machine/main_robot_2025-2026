#ifndef UDP_BUFFER_H
#define UDP_BUFFER_H

#include <pico.h>
#include <stdint.h>
#include <lwip/udp.h>

#define UDP_PAYLOAD_ANGLE_L_BYTE 0x00
#define UDP_PAYLOAD_ANGLE_H_BYTE 0x01
#define UDP_PAYLOAD_X_AXIS_SPEED_BYTE 0x02
#define UDP_PAYLOAD_Y_AXIS_SPEED_BYTE 0x03

// Callback function for writing data when udp package received
void __not_in_flash_func(udp_client_message_handler)(uint8_t *payload, uint16_t len, const ip_addr_t *addr, u16_t port);

#endif // UDP_BUFFER_H
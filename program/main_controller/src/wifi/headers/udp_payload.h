#ifndef UDP_BUFFER_H
#define UDP_BUFFER_H

#include <pico.h>
#include <stdint.h>
#include <lwip/udp.h>

// Callback function for writing data when udp package received
void __not_in_flash_func(udp_client_message_handler)(uint8_t *payload, uint16_t len, const ip_addr_t *addr, u16_t port);

#endif // UDP_BUFFER_H
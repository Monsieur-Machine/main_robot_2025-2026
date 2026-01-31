#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdint.h>
#include <lwip/udp.h>

#define UDP_CLIENT_PORT 4243

#define BUFFER_SIZE 1024

// Message callback deffinition
typedef void (*message_callback_t)(uint8_t *payload, uint16_t len, const ip_addr_t *addr, uint16_t port);

// Data in here is used by the SDK
typedef struct udp_client_t {
    struct udp_pcb *pcb;                // like this
    ip_addr_t local_addr;               // or this...
    uint16_t local_port;                // So don't remove them, even if they are not used explicitely in the program
    uint8_t recv_buffer[BUFFER_SIZE];   // Please (Do not even change their position)
    message_callback_t message_callback;
} udp_client_t;

// Init udp client, set callback to NULL for the default callback
void udp_client_init(void);
// Exit udp client
void udp_client_deinit(void);

#endif // UDP_CLIENT_H
#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdint.h>
#include <lwip/udp.h>

#define UDP_CLIENT_PORT 4243

typedef void (*message_callback_t)(uint8_t *payload, uint16_t len);

typedef struct udp_client_t {
    struct udp_pcb *pcb;
    message_callback_t message_callback;
} udp_client_t;

// Init udp client
int udp_client_init(void);
// Exit udp client
void udp_client_deinit(void);

#endif // UDP_CLIENT_H
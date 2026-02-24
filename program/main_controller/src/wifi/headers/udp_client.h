#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <stdint.h>
#include <lwip/udp.h>

#define UDP_CLIENT_PORT 4243

#define DATA_LEN (2 * sizeof(int16_t) + sizeof(uint8_t)) / sizeof(uint8_t)

typedef void (*message_callback_t)(uint8_t *payload, uint16_t len);

typedef struct udp_client_t {
    struct udp_pcb *pcb;
    message_callback_t message_callback;
    union {
        struct {
            struct {
                int16_t joystick_x;
                int16_t joystick_y;

                struct {
                    bool button_black : 1;
                    bool button_blue  : 1;
                    bool button_white : 1;
                    bool button_green : 1;
                    bool button_l     : 1;
                    bool button_r     : 1;
                } buttons;
            } inputs;
        } hard;

        uint8_t raw[DATA_LEN];
    } data;
} udp_client_t;

// Init udp client
int udp_client_init(void);
// Exit udp client
void udp_client_deinit(void);

#endif // UDP_CLIENT_H
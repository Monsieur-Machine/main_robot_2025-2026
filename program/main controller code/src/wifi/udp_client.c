#include "headers/udp_client.h"

#include <stdio.h>
#include "headers/udp_payload.h"

udp_client_t udp_client;

static inline void __not_in_flash_func(handle_receive)(struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    if(p->len >= 2)
    {
        uint8_t *payload = (uint8_t *)p->payload;
        uint16_t len = p->len;

        udp_client.message_callback(payload, len, addr, port);
    }

    pbuf_free(p);
}

static void __not_in_flash_func(udp_receive_callback)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    udp_client_t *udp_client_received_data = (udp_client_t *)arg;
    handle_receive(p, addr, port);
}

// Default callback func
static void __not_in_flash_func(default_message_callback)(uint8_t *payload, uint16_t len, const ip_addr_t *addr, uint16_t port)
{
    //printf("Received: len=%d from %s:%d\n", len, ipaddr_ntoa(addr), port);

    //for(uint i = 0; i < len; i++) printf("payload[%d]=%d | ", i, payload[i]);
    //puts("\n");

    printf(">Robot angle : %d\n", (int16_t)((payload[UDP_PAYLOAD_ANGLE_H_BYTE] << 8) | payload[UDP_PAYLOAD_ANGLE_L_BYTE]));
    printf(">Robot x speed : %d\n", (int8_t)payload[UDP_PAYLOAD_X_AXIS_SPEED_BYTE]);
    printf(">Robot y speed : %d\n", (int8_t)payload[UDP_PAYLOAD_Y_AXIS_SPEED_BYTE]);
}

void udp_client_init(void)
{
    //udp_client.message_callback = udp_client_message_handler;
    udp_client.message_callback = default_message_callback;

    udp_client.pcb = udp_new();
    if(udp_client.pcb == NULL)
    {
        puts("Error creating UDP client");
        return;
    }

    udp_recv(udp_client.pcb, udp_receive_callback, &udp_client);

    err_t err = udp_bind(udp_client.pcb, IP_ADDR_ANY, UDP_CLIENT_PORT);
    if(err != ERR_OK)
    {
        printf("Erreur bind UDP client: %d\n", err);
        return;
    }

    printf("UDP client started on port %d\n", UDP_CLIENT_PORT);
}

void udp_client_exit(void)
{
    if(udp_client.pcb)
    {
        udp_remove(udp_client.pcb);
        udp_client.pcb = NULL;
    }
}

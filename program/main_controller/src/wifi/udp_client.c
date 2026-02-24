#include "headers/udp_client.h"

#include <stdio.h>
#include <string.h>
#include "headers/robot.h"

static inline void message_callback(uint8_t *payload, uint16_t len)
{
    if(len != DATA_LEN)
        return;

    memcpy(robot.udp_client.data.raw, payload, len);
}

// Default callback func
static inline void default_message_callback(uint8_t *payload, uint16_t len)
{
    for(uint i = 0; i < len; i++)
        printf("payload[%d]=%d%s", i, payload[i], ((i + 1) == len) ? "" : " | ");

    puts("\n");
}

static inline void udp_receive_callback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    robot.udp_client.message_callback((uint8_t *)p->payload, p->len);
    pbuf_free(p);
}

int udp_client_init(void)
{
    robot.udp_client.message_callback = message_callback;
    //robot.udp_client.message_callback = default_message_callback;

    robot.udp_client.pcb = udp_new();
    if(robot.udp_client.pcb == NULL)
    {
        puts("Error creating UDP client");
        return -1;
    }

    udp_recv(robot.udp_client.pcb, udp_receive_callback, &robot.udp_client);

    err_t err = udp_bind(robot.udp_client.pcb, IP_ADDR_ANY, UDP_CLIENT_PORT);
    if(err)
    {
        printf("Error binding UDP client: %d\n", err);
        return -1;
    }

    printf("UDP client started on port %d\n", UDP_CLIENT_PORT);

    return 0;
}

void udp_client_deinit(void)
{
    if(robot.udp_client.pcb)
    {
        udp_remove(robot.udp_client.pcb);
        robot.udp_client.pcb = NULL;
    }
}

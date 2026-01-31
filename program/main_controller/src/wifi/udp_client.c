#include "headers/udp_client.h"

#include <stdio.h>
#include "headers/udp_payload.h"
#include "headers/robot.h"

// Default callback func
static void __not_in_flash_func(default_message_callback)(uint8_t *payload, uint16_t len)
{
    for(uint i = 0; i < len; i++)
        printf("payload[%d]=%d | ", i, payload[i]);

    puts("\n");
}

static void __not_in_flash_func(udp_receive_callback)(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    robot.udp_client.message_callback((uint8_t *)p->payload, p->len);
    pbuf_free(p);
}

int udp_client_init(void)
{
    //udp_client.message_callback = udp_client_message_handler;
    robot.udp_client.message_callback = default_message_callback;

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

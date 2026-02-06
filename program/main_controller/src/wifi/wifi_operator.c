#include "headers/wifi_operator.h"

#include <stdio.h>
#include <pico/cyw43_arch.h>
#include <lwip/netif.h>
#include <lwip/ip4_addr.h>

int wifi_operator_init(void)
{
    if(cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE))
    {
        puts("CYW43 no powersave mode failed");
        return -1;
    }

    // Station mode
    cyw43_arch_enable_sta_mode();

    for(int error_code = 1; error_code; )
    {
        printf("WiFi connection attempt to \"%s\"\n", WIFI_OPERATOR_SSID);

        error_code = cyw43_arch_wifi_connect_timeout_ms(WIFI_OPERATOR_SSID, WIFI_OPERATOR_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000);

        if(error_code)
        {
            const char *error_message;

            switch(error_code)
            {
              case PICO_ERROR_TIMEOUT:
                error_message = "Timeout reached";
                break;

              case PICO_ERROR_BADAUTH:
                error_message = "Bad password";
                break;

              case PICO_ERROR_CONNECT_FAILED:
                error_message = "Some reason";
                break;
            }

            printf("Connection failed : %s\n", error_message);
        }
    }

    puts("STA mode activated");

    return 0;
}

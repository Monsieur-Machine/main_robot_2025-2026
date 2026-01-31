#include "headers/wifi_operator.h"

#include <stdio.h>
#include <pico/cyw43_arch.h>
#include <lwip/netif.h>
#include <lwip/ip4_addr.h>

void wifi_operator_init(void)
{
    // Mode client
    cyw43_arch_enable_sta_mode();

    // Désactiver le mode d'économie d'énergie
    cyw43_wifi_pm(&cyw43_state, CYW43_NO_POWERSAVE_MODE);

    // Tentativs de connexion
    for(int error_code = 1; error_code; )
    {
        puts("WiFi connection attempt");

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

    puts("Connexion successfully etablished !");
}

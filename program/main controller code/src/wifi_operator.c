#include "include/wifi_operator.h"

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

    // Afficher les paramètres de connexion
    printf("Tentative de connexion à: SSID='%s'\n", WIFI_OPERATOR_SSID);

    // Configuration IP
    ip4_addr_t ip, netmask, gateway;
    IP4_ADDR(&ip, 192, 168, 128, 2);
    IP4_ADDR(&netmask, 255, 255, 255, 0);
    IP4_ADDR(&gateway, 192, 168, 128, 1);

    // Configuration réseau avant connexion
    netif_set_up(netif_default);
    netif_set_addr(netif_default, &ip, &netmask, &gateway);

    puts("Configuration IP effectuée");

    int connection_return = 1;

    // Tentativs de connexion
    do
    {
        puts("Tentative de connexion au réseau Wi-Fi...");
        connection_return = cyw43_arch_wifi_connect_timeout_ms(WIFI_OPERATOR_SSID, WIFI_OPERATOR_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000);

        if(connection_return)
        {
            const char *error_description;

            switch(connection_return)
            {
              case -1:
                error_description = "General error";
                break;

              case -2:
                error_description = "Point d'accès non trouvé";
                break;

              case -3:
                error_description = "Incorrect password";
                break;

              default:
                error_description = "Unknow error";
            }

            printf("Error: WiFi can't be connected - Error code: %d - %s\n", connection_return, error_description);
        }
    }
    while(connection_return);

    // Configuration de l'interface réseau
    if(!netif_default)
    {
        puts("Error: WiFi interface isn't accessible");
        return;
    }

    netif_set_up(netif_default);
    netif_set_link_up(netif_default);
    netif_set_addr(netif_default, &ip, &netmask, &gateway);

    puts("Connexion successfully etablished !");
}

idf_component_register(SRCS "main.c"
                       PRIV_REQUIRES esp_netif esp_eth ethernet_init esp_driver_gpio
                       INCLUDE_DIRS ".")

idf_component_register(SRCS 
    "driver_nrf24l01_interface_template.c"
    "driver_nrf24l01.c"
    "main.cpp"
    INCLUDE_DIRS 
    "." 
    REQUIRES driver esp_timer
)

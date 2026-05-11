#pragma once

#include "sdkconfig.h"

#define APP_FW_VERSION   "1.0.0"

#ifdef CONFIG_OTA_SERVER_URL
#  if CONFIG_OTA_SERVER_URL[4] != 's'
#    error "OTA_SERVER_URL must begin with https://"
#  endif
#endif

#define WIFI_CONNECTED_BIT   BIT0
#define WIFI_FAILED_BIT      BIT1

// config.h
#ifndef CONFIG_H
#define CONFIG_H

#include "WiFi.h"
#include "WiFiProv.h"
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"

// Event handler declaration
void SysProvEvent(arduino_event_t *sys_event);
void setupProvisioning();

#endif // CONFIG_H

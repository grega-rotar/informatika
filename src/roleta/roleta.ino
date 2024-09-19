#include "config.h"
#include "const.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char* TAG = "roleta.ino";
void setup() {
        ESP_LOGI(TAG, "starting");

        init_provisoning();
        // TODO: WiFi & MQTT FreeRTOS task to keep alive
}

void loop() {
        init_mqtt();
        delay(1000);
}
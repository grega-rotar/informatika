/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at
https://RandomNerdTutorials.com/esp32-wi-fi-provisioning-ble-arduino/

  Please read README.md file in this folder, or on the web:
https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiProv/examples/WiFiProv
  Note: This sketch takes up a lot of space for the app and may not be able to
flash with default setting on some chips. If you see Error like this: "Sketch
too big" In Arduino IDE go to: Tools > Partition scheme > chose anything that
has more than 1.4MB APP
   - for example "No OTA (2MB APP/2MB SPIFFS)"
*********/

/**
Use 2.x.x version of esp32 board or it will not work (checked on 4. sep)
**/
#include "config.h"
#include "const.h"

void setup() {
        Serial.begin(115200);
        pinMode(CONFIG_BTN_PIN, INPUT);
        setupProvisioning();
}

void loop() {
        if (digitalRead(CONFIG_BTN_PIN) == LOW) {
                Serial.println("CONFIG_BTN PRESSED");
                delay(5000);
                if (digitalRead(0) == LOW) {
                        Serial.println("RESETING_WIFI_PROVISONING");
                        wifi_prov_mgr_reset_provisioning();
                        ESP.restart();
                }
        }
}
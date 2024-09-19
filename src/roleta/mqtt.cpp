#include "WiFiClient.h"
#include <PubSubClient.h>
#include <WiFi.h>


WiFiClient wifiClient;
PubSubClient client(wifiClient);
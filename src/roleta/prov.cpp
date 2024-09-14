#include "config.h"
#include "const.h"
#include "esp_log.h"
#include "nvs.h"

// tag for log
static const char *TAG = "prov";

String prov_pop = PROV_POP;
String prov_device_name = PROV_DEVICE_NAME;

const char *prov_service_key = NULL; // Password for SofAP method
// Should provisioning data reset on reboot?
// does not make sense
// FIXME: should be safely removed
bool prov_reset = false;

class ProvDetails {
      private:
        String pop;
        String device_name;

      public:
        ProvDetails(String p, String d) {
                pop = pop;
                device_name = d;
        }
        void setPop(String p) { pop = p; }
        String getPop() { return pop; }

        void setDeviceName(String d) { device_name = d; }
        String getDeviceName() { return device_name; }
};

ProvDetails provDetails(PROV_POP, PROV_DEVICE_NAME);

bool logAndCheckErr(esp_err_t err) {
        if (err == ESP_OK)
                return false;
        ESP_LOGE(TAG, "NVS error: %s", esp_err_to_name(err));
        return true;
}

void getProvDetails(ProvDetails *prov) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
            err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);

        nvs_handle_t nvs_handle;
        err = nvs_open("prov", NVS_READONLY, &nvs_handle);
        logAndCheckErr(err);

        uint32_t pop = 0;
        err = nvs_get_u32(nvs_handle, "pop", &pop);
        logAndCheckErr(err);
        ppop = PROV_PREFIX + String(pop, HEX);

        uint32_t device_name = 0;
        err = nvs_get_u32(nvs_handle, "dn", &device_name);
        logAndCheckErr(err);
        pdevice_name = String(device_name, HEX);

        nvs_close(nvs_handle);
}

bool getAndCheckProvDetails(String &ppop, String &pdevice_name) {
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
            err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);

        nvs_handle_t nvs_handle;
        err = nvs_open("prov", NVS_READONLY, &nvs_handle);
        if (logAndCheckErr(err)) {
                return false;
        }

        uint32_t pop = 0;
        err = nvs_get_u32(nvs_handle, "pop", &pop);
        logAndCheckErr(err);
        ppop = PROV_PREFIX + String(pop, HEX);

        uint32_t device_name = 0;
        err = nvs_get_u32(nvs_handle, "dn", &device_name);
        logAndCheckErr(err);
        pdevice_name = String(device_name, HEX);

        nvs_close(nvs_handle);

        return (pop != 0 && device_name != 0);
}

void initalizeProvDetails() {

        // checking if prov details have been created and saved
        if (getAndCheckProvDetails(prov_pop, prov_device_name)) {
                ESP_LOGI(TAG, "prov details exist", "");
                return;
        }

        // creating new prov credentials
        // creating
        esp_err_t err = nvs_flash_init();
        if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
            err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                ESP_ERROR_CHECK(nvs_flash_erase());
                err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(err);

        // opening "prov" namespace
        nvs_handle_t nvs_handle;
        err = nvs_open("prov", NVS_READWRITE, &nvs_handle);
        logAndCheckErr(err);

        // getting random 32 bit integars
        uint32_t pop = esp_random();
        uint32_t device_name = esp_random();
        err = nvs_set_u32(nvs_handle, "pop", pop);
        logAndCheckErr(err);
        err = nvs_set_u32(nvs_handle, "dn", device_name);
        logAndCheckErr(err);

        // Commit written value to NVS storage
        err = nvs_commit(nvs_handle);
        logAndCheckErr(err);

        // closing NVS
        nvs_close(nvs_handle);
}

void SysProvEvent(arduino_event_t *sys_event) {
        switch (sys_event->event_id) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                Serial.print("\nConnected IP address : ");
                Serial.println(
                    IPAddress(sys_event->event_info.got_ip.ip_info.ip.addr));
                break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                Serial.println(
                    "\nDisconnected. Connecting to the AP again... ");
                break;
        case ARDUINO_EVENT_PROV_START:
                Serial.println(
                    "\nProvisioning started\nGive Credentials of your access "
                    "point using smartphone app");
                break;
        case ARDUINO_EVENT_PROV_CRED_RECV: {
                Serial.println("\nReceived Wi-Fi credentials");
                Serial.print("\tSSID : ");
                Serial.println(
                    (const char *)sys_event->event_info.prov_cred_recv.ssid);
                Serial.print("\tPassword : ");
                Serial.println((char const *)sys_event->event_info
                                   .prov_cred_recv.password);
                break;
        }
        case ARDUINO_EVENT_PROV_CRED_FAIL: {
                Serial.println(
                    "\nProvisioning failed!\nPlease reset to factory and retry "
                    "provisioning\n");
                if (sys_event->event_info.prov_fail_reason ==
                    WIFI_PROV_STA_AUTH_ERROR)
                        Serial.println("\nWi-Fi AP password incorrect");
                else
                        Serial.println("\nWi-Fi AP not found....Add API \" "
                                       "nvs_flash_erase() \" "
                                       "before beginProvision()");
                break;
        }
        case ARDUINO_EVENT_PROV_CRED_SUCCESS:
                Serial.println("\nProvisioning Successful");
                break;
        case ARDUINO_EVENT_PROV_END:
                Serial.println("\nProvisioning Ends");
                break;
        default:
                break;
        }
}

void setupProvisioning() {

        initalizeProvDetails();

        // assing WiFi event function
        WiFi.onEvent(SysProvEvent);

        Serial.println("Begin Provisioning using BLE");
        // Sample uuid that user can pass during provisioning using BLE
        uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf,
                            0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
        WiFiProv.beginProvision(
            WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
            WIFI_PROV_SECURITY_1, prov_pop.c_str(), prov_device_name.c_str(),
            prov_service_key, uuid, prov_reset);
        log_d("ble qr");
        WiFiProv.printQR(prov_device_name.c_str(), prov_pop.c_str(), "ble");
}

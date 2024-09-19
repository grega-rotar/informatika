#include "config.h"
#include "const.h"
#include "esp_err.h"
#include "esp_log.h"
#include "nvs.h"

// tag for log
static const char *TAG = "prov";
static const char *prov_service_key = NULL; // Password for SofAP method, NOT USED

class ProvDetails {
      private:
        String pop;
        String device_name;

      public:
        ProvDetails(String p, String d) {
                pop = p;
                device_name = d;
        }
        void setPop(String p) { pop = p; }
        char *getPop() {
                unsigned char bufferSize = pop.length() + 1;
                char *buffer = new char[bufferSize];
                pop.toCharArray(buffer, bufferSize);
                return buffer;
        }

        void setDeviceName(String d) { device_name = PROV_PREFIX + d; }
        char *getDeviceName() {
                unsigned char bufferSize = device_name.length() + 1;
                char *buffer = new char[bufferSize];
                device_name.toCharArray(buffer, bufferSize);
                return buffer;
        }
};

// setting provDetails to default values
ProvDetails provDetails(PROV_POP, PROV_DEVICE_NAME);

// function to log errors
// DEPRECATED: it's not so useful
static bool log_and_check_err(esp_err_t err) {
        if (err == ESP_OK)
                return false;
        ESP_LOGE(TAG, "NVS error: %s", esp_err_to_name(err));
        return true;
}

static void init_nvs(esp_err_t *err) {
        *err = nvs_flash_init();

        // check the initalization errors
        // ESP_ERR_NVS_NO_FREE_PAGES: No free pages available in the NVS
        // partition. ESP_ERR_NVS_NEW_VERSION_FOUND: A new version of the NVS
        // library has been detected, indicating compatibility issues.
        if (*err == ESP_ERR_NVS_NO_FREE_PAGES ||
            *err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
                // earse the NVS partition to clear corrupted data
                ESP_ERROR_CHECK(nvs_flash_erase());

                // trying to initalize again
                *err = nvs_flash_init();
        }
        ESP_ERROR_CHECK(*err);
}

// FIXME: add option to open as READ_ONLY
static void open_nvs_namespace(const char *nvs_namespace, nvs_handle_t *nvs_handle,
                        esp_err_t *err) {
        *err = nvs_open(nvs_namespace, NVS_READWRITE, nvs_handle);
        if (*err != ESP_OK) {
                ESP_LOGE(TAG, "nvs_open error: %s", esp_err_to_name(*err));
                nvs_close(*nvs_handle);
                return;
        }
}

static void init_prov_details(ProvDetails *pProvDetails) {
        esp_err_t err;
        init_nvs(&err);

        // opening "prov" namespace
        nvs_handle_t nvs_handle;
        open_nvs_namespace("prov", &nvs_handle, &err);

        // generating and saving data to NVS
        uint32_t pop = esp_random();
        err = nvs_set_u32(nvs_handle, "pop", pop);
        if (err != ESP_OK) {
                nvs_close(nvs_handle);
                return;
        }
        pProvDetails->setPop(String(pop, HEX));

        uint32_t device_name = esp_random();
        err = nvs_set_u32(nvs_handle, "dn", device_name);
        if (err != ESP_OK) {
                nvs_close(nvs_handle);
                return;
        }
        pProvDetails->setDeviceName(String(device_name, HEX));

        // Commit written value to NVS storage
        err = nvs_commit(nvs_handle);
        log_and_check_err(err);

        // closing NVS
        nvs_close(nvs_handle);
        return;
}

static void handle_prov_nvs_err(esp_err_t err, ProvDetails *pProvDetails) {
        if (err == ESP_OK) {
                return;
        };

        if (err == ESP_ERR_NVS_NOT_FOUND) {
                // prov details don't exist in NVS
                // creating new ones
                init_prov_details(pProvDetails);
        }
}

static void ensure_prov_details(ProvDetails *pProvDetails) {
        // FIXME: add error handling
        // Attempt to initalize the NVS (Non-Voltaile Storage) flash
        esp_err_t err;
        init_nvs(&err);

        // opening "prov" namespace
        nvs_handle_t nvs_handle;
        open_nvs_namespace("prov", &nvs_handle, &err);

        uint32_t pop = 0;
        err = nvs_get_u32(nvs_handle, "pop", &pop);
        pProvDetails->setPop(String(pop, HEX));
        // check if data does not exist and creating new
        handle_prov_nvs_err(err, pProvDetails);

        uint32_t device_name = 0;
        err = nvs_get_u32(nvs_handle, "dn", &device_name);
        pProvDetails->setDeviceName(String(device_name, HEX));
        // check if data does not exist and creating new
        handle_prov_nvs_err(err, pProvDetails);

        nvs_close(nvs_handle);
        return;
}

void SysProvEvent(arduino_event_t *sys_event) {
        switch (sys_event->event_id) {
                case ARDUINO_EVENT_WIFI_STA_GOT_IP:
                        Serial.print("\nConnected IP address : ");
                        Serial.println(IPAddress(
                            sys_event->event_info.got_ip.ip_info.ip.addr));
                        break;
                case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
                        Serial.println(
                            "\nDisconnected. Connecting to the AP again... ");
                        break;
                case ARDUINO_EVENT_PROV_START:
                        Serial.println("\nProvisioning started\nGive "
                                       "Credentials of your access "
                                       "point using smartphone app");
                        break;
                case ARDUINO_EVENT_PROV_CRED_RECV: {
                        Serial.println("\nReceived Wi-Fi credentials");
                        Serial.print("\tSSID : ");
                        Serial.println((const char *) sys_event->event_info
                                           .prov_cred_recv.ssid);
                        Serial.print("\tPassword : ");
                        Serial.println((char const *) sys_event->event_info
                                           .prov_cred_recv.password);
                        break;
                }
                case ARDUINO_EVENT_PROV_CRED_FAIL: {
                        Serial.println("\nProvisioning failed!\nPlease reset "
                                       "to factory and retry "
                                       "provisioning\n");
                        if (sys_event->event_info.prov_fail_reason ==
                            WIFI_PROV_STA_AUTH_ERROR)
                                Serial.println("\nWi-Fi AP password incorrect");
                        else
                                Serial.println(
                                    "\nWi-Fi AP not found....Add API \" "
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

void init_provisoning() {
        ensure_prov_details(&provDetails);

        // assigning WiFi event function
        WiFi.onEvent(SysProvEvent);

        Serial.println("Begin Provisioning using BLE");
        // Sample uuid that user can pass during provisioning using BLE
        uint8_t uuid[16] = {0xb4, 0xdf, 0x5a, 0x1c, 0x3f, 0x6b, 0xf4, 0xbf,
                            0xea, 0x4a, 0x82, 0x03, 0x04, 0x90, 0x1a, 0x02};
        WiFiProv.beginProvision(
            WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM,
            WIFI_PROV_SECURITY_1, provDetails.getPop(),
            provDetails.getDeviceName(), prov_service_key, uuid, false);
        log_d("ble qr");
        WiFiProv.printQR(provDetails.getDeviceName(), provDetails.getPop(),
                         "ble");
}

#include "wifi.h"
#include <ESP8266WiFi.h>

#define LOG_PREFIX    "WiFi"
#include "log.h"

static const char* ssid = "SSID";
static const char* password = "PASSWORD";

static bool prevStatus = false;

void wifi_init(void)
{
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.setAutoConnect(true);
    WiFi.begin(ssid, password);
    log_printf("Connecting to %s\n", ssid);
}

void wifi_loop(void)
{
    bool status = WiFi.isConnected();

    if (status != prevStatus) {
        if (status) {
            log_printf("Connected to %s\n", str2cstr(WiFi.SSID()));
            log_printf("IP: %s\n", obj2cstr(WiFi.localIP()));
        } else {
            log_printf("Disconnected!\n");
        }

        prevStatus = status;
    }
}

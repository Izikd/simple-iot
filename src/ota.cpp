#include "ota.h"
#include <ArduinoOTA.h>
#include "watchdog.h"

#define LOG_PREFIX    "OTA"
#include "log.h"

void ota_init(void)
{
    ArduinoOTA.setPort(OTA_PORT);

    // No authentication by default
    // ArduinoOTA.setPassword((const char *)"123");

    ArduinoOTA.onStart([]() {
        log_printf("Start\n");
    });
    ArduinoOTA.onEnd([]() {
        log_printf("\nEnd\n");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        log_printf("Progress: %u%%\r", (progress / (total / 100)));
        watchdog_feed();
    });
    ArduinoOTA.onError([](ota_error_t error) {
        const char *errstr;
        
        switch (error)
        {
            case OTA_AUTH_ERROR:
                errstr = "Auth Failed";
                break;
            case OTA_BEGIN_ERROR:
                errstr = "Begin Failed";
                break;
            case OTA_CONNECT_ERROR:
                errstr = "Connect Failed";
                break;
            case OTA_RECEIVE_ERROR:
                errstr = "Receive Failed";
                break;
            case OTA_END_ERROR:
                errstr = "End Failed";
                break;
            default:
                errstr = "N/A";
                break;
        }
            
        log_printf("Error[%u]: %s\n", error, errstr);
    });

    ArduinoOTA.begin();

    log_printf("Ready at %s:%d\n", str2cstr(ArduinoOTA.getHostname()), OTA_PORT);
}

void ota_loop(void)
{
    ArduinoOTA.handle();
}
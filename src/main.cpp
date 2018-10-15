#include <Arduino.h>

#define LOG_PREFIX    "Main"
#include "log.h"

#include "watchdog.h"
#include "cli.h"
#include "cli_basic.h"
#include "wifi.h"
#include "ota.h"

void setup() {
    log_init();
    log_printf("SimpleIoT\n");
    watchdog_init();
    cli_init();
    cli_basic_init();

    wifi_init();
    ota_init();
}

void loop() {
    wifi_loop();
    ota_loop();
    cli_loop();

    watchdog_feed();
}

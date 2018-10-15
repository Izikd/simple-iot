#include <Arduino.h>

#define LOG_PREFIX    "Main"
#include "log.h"

#include "watchdog.h"
void setup() {
    log_init();
    log_printf("SimpleIoT\n");
    watchdog_init();
}

void loop() {
    watchdog_feed();
}

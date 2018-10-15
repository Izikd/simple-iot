#include <Arduino.h>

#define LOG_PREFIX    "Main"
#include "log.h"

#include "watchdog.h"
#include "cli.h"
void setup() {
    log_init();
    log_printf("SimpleIoT\n");
    watchdog_init();
    cli_init();
}

void loop() {
    cli_loop();

    watchdog_feed();
}

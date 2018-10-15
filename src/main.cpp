#include <Arduino.h>

#define LOG_PREFIX    "Main"
#include "log.h"

void setup() {
    log_init();
    log_printf("SimpleIoT\n");
}

void loop() {
}
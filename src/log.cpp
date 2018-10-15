#include <Arduino.h>
#include "log.h"

void log_init(void)
{
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    // Serial.begin(74800);
    Serial.println();
}

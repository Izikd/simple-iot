#ifndef __LOG_H__
#define __LOG_H__

#include <HardwareSerial.h>

/** Define module name before including this file for log prefix */
#ifndef LOG_PREFIX
#define LOG_PREFIX    "???"
#endif

#define log_printf(fmt, ...)     \
    Serial.printf("%-6s: " fmt, LOG_PREFIX, ##__VA_ARGS__)

#ifdef LOG_DEBUG
#define log_debug_printf(...)   log_printf(##__VA_ARGS__)
#else
#define log_debug_printf(...)
#endif

#define str2cstr(o)            \
    (o).c_str()

#define obj2cstr(o)            \
    str2cstr((o).toString())

void log_init(void);

#endif /* __LOG_H__ */

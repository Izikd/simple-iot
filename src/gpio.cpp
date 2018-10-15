#include "simple_gpio.h"
#include "cli.h"
#include <Arduino.h>

#define LOG_PREFIX    "GPIO"
#include "log.h"

static int gpio_handler(int argc, char** const argv)
{
    uint8_t gpio;
    uint8_t val;

    if (argc < 2)
        return -1;

    gpio = atoi(argv[1]);

    if (argc > 2) {
        val = atoi(argv[2]);

        digitalWrite(gpio, val);
    } else {
        pinMode(gpio, INPUT);

        val = digitalRead(gpio);
    }

    cli_printf("GPIO %u = %u\n", gpio, val);

    return 0;
}

static struct cli_entry gpio_cli = {
    .cmd = "gpio",
    .handler = gpio_handler,
    .help = "gpio <num> [set_val]\n"
            "Digital GPIOs setter/getter. When provoding the set_val it will write to GPIO, otherwise read."
};

void gpio_init(void)
{
    cli_entry_add(&gpio_cli);

    log_printf("Ready\n");
}
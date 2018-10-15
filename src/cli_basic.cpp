#include "cli_basic.h"
#include "cli.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"
}

#define LOG_PREFIX "CLI_Bs"
#include "log.h"

/* Reset */
static int reset_handler(int argc, char **const argv)
{
    ESP.restart();

    return 0;
}

static struct cli_entry reset_cli = {
    .cmd = "reset",
    .handler = reset_handler,
    .help = "Reset board"
};

/* Sleep */
#if 0
static int sleep_handler(int argc, char **const argv)
{
    unsigned int t;
    unsigned int rf_mode;

    if (argc < 3)
        return -1;

    t = atoi(argv[1]);

    rf_mode = atoi(argv[2]);
    if (rf_mode > 4)
    {
        cli_printf("Invalid RF mode\n");
        return 1;
    }

    cli_printf("deepSleep(%u, %u)\n", t, rf_mode);

    ESP.deepSleep(t, (RFMode)rf_mode);

    return 0;
}
#endif


extern "C" void esp_yield();

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF

uint8_t last_opmode;

void fpm_wakup_cb_func1(void)
{
    cli_printf("%s\n", __func__);

    wifi_fpm_close();              // disable force sleep function
    wifi_set_opmode(last_opmode); // set station mode
    // wifi_station_connect();        // connect to AP
    WiFi.begin();
}
#ifndef SLEEP_MAX
// Wakeup till time out.
int user_func(uint32_t t_ms)
{
    last_opmode = wifi_get_opmode();

    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE);                 // set WiFi mode to null mode.
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);     // light sleep
    wifi_fpm_open();                            // enable force sleep
    wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1); // Set wakeup callback
    wifi_fpm_do_sleep(t_ms * 1000);

    delay(t_ms + 5);
    // esp_yield();

    return 0;
}
#else
// Or wake up by GPIO
void user_func(...)
{
    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE);             // set WiFi mode to null mode.
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); // light sleep
    wifi_fpm_open();                        // enable force sleep
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);
    wifi_enable_gpio_wakeup(12, GPIO_PIN_INTR_LOLEVEL);
    wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1); // Set wakeup callback
    wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);
}
#endif

static int sleep_handler2(int argc, char **const argv)
{
    unsigned int t_ms;

    if (argc < 2)
        return -1;

    t_ms = atoi(argv[1]);

    cli_printf("user_func(%u)\n", t_ms);

    return user_func(t_ms);

    return 0;
}

static struct cli_entry sleep_cli = {
    .cmd = "sleep",
    .handler = sleep_handler2,
    .help = "sleep <us> <rf_mode>"
};

static int info_handler(int argc, char **const argv)
{
    cli_printf("ESP8266 SDK: %s\n", system_get_sdk_version());
    cli_printf("Chip ID: 0x%08x\n", system_get_chip_id());
    cli_printf("VDD 3.3: %u\n", system_get_vdd33());
    cli_printf("CPU Freq.: %u MHz\n", system_get_cpu_freq());
    cli_printf("Mem. info:\n");
    system_set_os_print(true);
    system_print_meminfo();

    return 0;
}

static struct cli_entry info_cli = {
    .cmd = "info",
    .handler = info_handler,
    .help = "System information"
};

void cli_basic_init(void)
{
    cli_entry_add(&reset_cli);
    cli_entry_add(&sleep_cli);
    cli_entry_add(&info_cli);

    log_printf("Ready\n");
}
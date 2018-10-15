#include "watchdog.h"
extern "C" {
#include "user_interface.h"
}
#define LOG_PREFIX    "WD"
#include "log.h"

#define WATCHDOG_CHECK_TIME     (WATCHDOG_TIMEOUT >> 1)

static uint32_t last_feed;
static os_timer_t wd_check_timer;

static void wd_check_cb(void *arg)
{
    uint32_t cur_time;
    uint32_t diff_time;

    cur_time = system_get_time();

    /* Forge time diff, even if overflow occured */
    if (last_feed < cur_time) {
        diff_time = cur_time - last_feed;
    } else {
        diff_time = ~0 - last_feed;
        diff_time += cur_time;
    }

    if (diff_time > (WATCHDOG_TIMEOUT * 1000)) {
        log_printf("Timeout! Restarting...\n");
        system_restart();
    }
}

void watchdog_init(void)
{
    watchdog_feed();

    os_timer_setfn(&wd_check_timer, wd_check_cb, NULL);
    os_timer_arm(&wd_check_timer, WATCHDOG_CHECK_TIME, true);
}

void watchdog_feed()
{
    last_feed = system_get_time();
}

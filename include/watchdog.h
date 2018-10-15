#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#define WATCHDOG_TIMEOUT    3000 /* mSec */

void watchdog_init(void);
void watchdog_feed(void);

#endif // __WATCHDOG_H__

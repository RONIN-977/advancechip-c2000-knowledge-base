#ifndef UPGRADE_ENTRY_H
#define UPGRADE_ENTRY_H
/* Call init once after Device_init. Call poll frequently (<1 ms preferred).
 * In an existing application, adapt this to that application's own CAN
 * receive dispatcher instead of configuring the peripheral a second time.
 * Quiesce PWM/relays/control outputs BEFORE accepting an upgrade request. */
void upgrade_entry_init(void);
void upgrade_entry_poll(void);
#endif

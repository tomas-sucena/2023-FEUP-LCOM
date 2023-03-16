#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdint.h>

/* IRQ */
#define KBD_IRQ 1

// for interrupts
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
int (kbd_enable_int)();

void (kbd_get_scancode)(uint8_t *scancode);

#endif // _LCOM_KEYBOARD_H_

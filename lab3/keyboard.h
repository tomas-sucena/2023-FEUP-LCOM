#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdint.h>

#include "i8042.h"

// for interrupts
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
void (kbd_ih)();

// parse keyboard status
int (kbd_read_status)(uint8_t *st);
int (kbd_parse_status)(uint8_t st);

#endif // _LCOM_KEYBOARD_H_

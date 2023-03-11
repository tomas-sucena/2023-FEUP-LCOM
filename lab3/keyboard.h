#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdint.h>

#include "i8042.h"

enum kbd_status {
    NO_ERROR,
    PARITY_ERROR,
    TIMEOUT_ERROR,
    MOUSE_DATA,
    IBF_FULL,
    OBF_FULL
};

// for interrupts
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();
void (kbd_get_scancode)(uint8_t *scancode);

// parse keyboard status
int (kbd_get_status)(uint8_t *st);
enum kbd_status (kbd_parse_status)(uint8_t st);

#endif // _LCOM_KEYBOARD_H_

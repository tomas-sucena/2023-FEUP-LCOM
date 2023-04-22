#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

#include "KBC.h"

/* IRQ */
#define KBD_IRQ 1

/* SCANCODES */
#define KBD_ESC_BREAKCODE 0x81
#define KBD_2B_SCANCODE 0xE0
#define breakcode(makecode) (makecode | BIT(7))
#define is_makecode(scancode) !(scancode & BIT(7))

typedef struct {
    uint8_t scancode;
    bool valid;
    bool two_byte;
} kbd_data;

// for interrupts
int (kbd_enable_int)(uint32_t wait_ticks);
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();

void (kbd_get_scancode)(kbd_data* data, uint32_t wait_ticks);

#endif // _LCOM_KEYBOARD_H_

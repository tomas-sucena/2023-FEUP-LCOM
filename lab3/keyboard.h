#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <stdbool.h>
#include <stdint.h>

/* IRQ */
#define KBD_IRQ 1

/* SCANCODES */
#define KBD_ESC_BREAKCODE 0x81
#define KBD_2B_SCANCODE 0xE0
#define breakcode(makecode) (makecode | BIT(7))
#define is_makecode(scancode) !(scancode & BIT(7))

struct kbd_data {
    uint8_t scancode;
    bool valid;
    bool two_byte;
};

// for interrupts
int (kbd_enable_int)(int wait_seconds);
int (kbd_subscribe_int)(uint8_t *bit_no);
int (kbd_unsubscribe_int)();

void (kbd_get_scancode)(struct kbd_data* data, int wait_seconds);

#endif // _LCOM_KEYBOARD_H_

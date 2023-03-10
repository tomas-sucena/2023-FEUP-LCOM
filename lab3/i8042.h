#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/* IRQ */
#define KBD_IRQ 1

/* SCANCODES */
#define KBD_ESC_BREAKCODE 0x81
#define breakcode(makecode) makecode | BIT(7)
#define is_makecode(scancode) !(scancode & BIT(7))

/* REGISTERS */
#define KBD_STATUS_REG 0x64
#define KBD_OBF 0x60

/* STATUS BITS */
#define KBD_PARITY_ERR BIT(7)
#define KBD_TIMEOUT_ERR BIT(6)
#define KBD_MOUSE_DATA BIT(5)
#define KBD_IBF_FULL BIT(1)
#define KBD_OBF_FULL BIT(0)

#endif // _LCOM_I8042_H_

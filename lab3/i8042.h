#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>
#include <stdbool.h>

/* IRQ */
#define KBD_IRQ 1

/* SCANCODES */
#define KBD_ESC_BREAKCODE 0x81
#define KBD_2B_SCANCODE 0xE0
#define breakcode(makecode) (makecode | BIT(7))
#define is_makecode(scancode) !(scancode & BIT(7))

/* REGISTERS */
#define KBD_STATUS_REG 0x64
#define KBD_COMMAND_REG 0x64 // for writing KBC commands
#define KBD_OBF 0x60

/* STATUS BITS */
#define KBD_PARITY_ERROR BIT(7)
#define KBD_TIMEOUT_ERROR BIT(6)
#define KBD_MOUSE_DATA BIT(5)
#define KBD_IBF_FULL BIT(1)
#define KBD_OBF_FULL BIT(0)

/* KBC COMMAND BITS */
#define KBC_DISABLE_MOUSE BIT(5)
#define KBC_DISABLE_KBD BIT(4)
#define KBC_ENABLE_MOUSE_INT BIT(1)
#define KBC_ENABLE_KBD_INT BIT(0)

#endif // _LCOM_I8042_H_

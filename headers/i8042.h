#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>
#include <stdbool.h>

/* REGISTERS */
#define KBC_STATUS_REG 0x64
#define KBC_COMMAND_REG 0x64 // for writing KBC commands
#define KBC_IN_BUF 0x60
#define KBC_OUT_BUF 0x60

/* KBC STATUS BITS */
#define KBC_PARITY_ERROR BIT(7)
#define KBC_TIMEOUT_ERROR BIT(6)
#define KBC_MOUSE_DATA BIT(5)
#define KBC_IBF BIT(1)
#define KBC_OBF BIT(0)

/* KBC COMMAND BITS */
#define KBC_DISABLE_MOUSE BIT(5)
#define KBC_DISABLE_KBD BIT(4)
#define KBC_ENABLE_MOUSE_INT BIT(1)
#define KBC_ENABLE_KBD_INT BIT(0)

#endif // _LCOM_I8042_H_

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdint.h>

#include "i8042.h"

#define KBC_READ 0x20
#define KBC_WRITE 0x60

int (kbc_get_command)(uint8_t* command);
int (kbc_write_command)(uint8_t command);
int (kbc_enable_int)();

#endif // _LCOM_KBC_H_

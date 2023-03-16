#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <stdint.h>

/* IRQ */
#define MOUSE_IRQ 12

int (mouse_subscribe_int)(uint8_t* bit_no);
int (mouse_unsubscribe_int)();

#endif // _LCOM_MOUSE_H_
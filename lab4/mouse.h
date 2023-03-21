#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <lcom/lab4.h>
#include <stdbool.h>
#include <stdint.h>

/* IRQ */
#define MOUSE_IRQ 12

/* ACKNOWLEDGEMENT BYTES */
#define MOUSE_SUCCESS_BYTE 0xFA
#define MOUSE_ERROR_BYTE 0xFE
#define MOUSE_TWICE_ERROR_BYTE 0xFC

#define is_first_byte(byte) (byte & BIT(3))
#define sign_extend(byte) (0xFF00 | byte)

/* MOUSE BITS */
#define MOUSE_Y_OVFL BIT(7)
#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_MSB BIT(5)
#define MOUSE_X_MSB BIT(4)
#define MOUSE_MIDDLE_BUTTON BIT(2)
#define MOUSE_RIGHT_BUTTON BIT(1)
#define MOUSE_LEFT_BUTTON BIT(0)

int (mouse_subscribe_int)(uint8_t* bit_no);
int (mouse_unsubscribe_int)();
int (mouse_enable_stream_mode)(int wait_ticks);
int (mouse_disable_stream_mode)(int wait_ticks);

void (mouse_get_data)(struct packet* pp, int wait_ticks);
void (mouse_parse_packet)(struct packet* pp);

#endif // _LCOM_MOUSE_H_

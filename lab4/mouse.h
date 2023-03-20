#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_

#include <stdbool.h>
#include <stdint.h>

/* IRQ */
#define MOUSE_IRQ 12

#define SUCCESS_BYTE 0xFA
#define ERROR_BYTE 0xFE
#define TWICE_ERROR_BYTE 0xFC

#define is_first_byte(byte) (byte & BIT(3))

/* MOUSE BITS */
#define MOUSE_Y_OVFL BIT(7)
#define MOUSE_X_OVFL BIT(6)
#define MOUSE_Y_MSB BIT(5)
#define MOUSE_X_MSB BIT(4)
#define MOUSE_MIDDLE_BUTTON BIT(2)
#define MOUSE_RIGHT_BUTTON BIT(1)
#define MOUSE_LEFT_BUTTON BIT(0)

struct packet {
  uint8_t bytes[3]; // mouse packet raw bytes
  bool rb, mb, lb;  // right, middle and left mouse buttons pressed
  int16_t delta_x;  // mouse x-displacement: rightwards is positive
  int16_t delta_y;  // mouse y-displacement: upwards is positive
  bool x_ov, y_ov;  // mouse x-displacement and y-displacement overflows
};

int (mouse_subscribe_int)(uint8_t* bit_no);
int (mouse_unsubscribe_int)();

void (mouse_get_data)(struct packet* pp, int wait_seconds);
void (mouse_parse_packet)(struct packet* pp);

#endif // _LCOM_MOUSE_H_

#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdbool.h>
#include <stdint.h>

/* KBC COMMANDS */
#define KBC_READ 0x20
#define KBC_WRITE 0x60
#define KBC_FORWARD_TO_MOUSE 0xD4
#define KBC_ENABLE 0xF4
#define KBC_DISABLE 0xF5

#define DELAY_US 20000

struct kbc_status {
    bool parity_error;
    bool timeout_error;
    bool mouse_data;
    bool ibf_full;
    bool obf_full;
};

static uint8_t st; // status

// status functions
int (kbc_get_status)();
struct kbc_status (kbc_parse_status)();
bool (kbc_can_write)();
bool (kbc_can_read)();
int (kbc_delay_write)(uint32_t wait_ticks);
int (kbc_delay_read)(uint32_t wait_ticks);

// command byte functions
int (kbc_write_command)(uint8_t command, uint32_t wait_ticks);
int (kbc_get_command_byte)(uint8_t* command, uint32_t wait_ticks);
int (kbc_set_command_byte)(uint8_t command, uint32_t wait_ticks);

// I/O buffer functions
int (kbc_read_out_buf)(uint8_t* data, uint32_t wait_ticks);
int (kbc_write_in_buf)(uint8_t data, uint32_t wait_ticks);

// keyboard functions
int (kbc_enable_kbd_int)(uint32_t wait_ticks);

#endif // _LCOM_KBC_H_

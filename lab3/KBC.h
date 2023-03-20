#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdbool.h>
#include <stdint.h>

#define KBC_READ 0x20
#define KBC_WRITE 0x60
#define DELAY_US 20000

struct kbc_status {
    bool parity_error;
    bool timeout_error;
    bool mouse_data;
    bool ibf_full;
    bool obf_full;
};

// OBF functions
int (kbc_get_status)(uint8_t* st);
struct kbc_status (kbc_parse_status)(uint8_t st);
bool (kbc_can_write)(uint8_t *st);
bool (kbc_can_read)(uint8_t *st);
int (kbc_delay_write)(int wait_seconds, uint8_t *st);
int (kbc_delay_read)(int wait_seconds, uint8_t *st);

int (kbc_read_obf)(uint8_t* data, int wait_seconds);
int (kbc_get_command)(uint8_t* command, int wait_seconds);
int (kbc_write_command)(uint8_t command, int wait_seconds);
int (kbc_enable_int)(int wait_seconds);

#endif // _LCOM_KBC_H_

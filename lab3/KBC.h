#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <stdint.h>

#define KBC_READ 0x20
#define KBC_WRITE 0x60
#define DELAY_US 20000

enum kbc_status {
    NO_ERROR,
    PARITY_ERROR,
    TIMEOUT_ERROR,
    MOUSE_DATA,
    IBF_FULL,
    OBF_FULL
};

// OBF functions
int kbc_get_status(uint8_t* status);
enum kbc_status kbc_parse_status(uint8_t status);
int kbc_read_obf(uint8_t* data, int timeout);

int (kbc_get_command)(uint8_t* command);
int (kbc_write_command)(uint8_t command);
int (kbc_enable_int)();

#endif // _LCOM_KBC_H_

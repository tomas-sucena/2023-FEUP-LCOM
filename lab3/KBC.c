#include <lcom/lcf.h>

#include "KBC.h"
#include "i8042.h"

extern uint32_t acc;

int (kbc_get_status)(uint8_t* status){
    if (status == NULL) return 1;

    ++acc;
    return util_sys_inb(KBC_STATUS_REG, status);
}

enum kbc_status (kbc_parse_status)(uint8_t st){
    if (st & KBC_PARITY_ERROR)
        return PARITY_ERROR;

    if (st & KBC_TIMEOUT_ERROR)
        return TIMEOUT_ERROR;

    if (st & KBC_MOUSE_DATA)
        return MOUSE_DATA;

    if (!(st & KBC_OBF_FULL))
        return OBF_FULL;

    return NO_ERROR;
}

int (kbc_read_obf)(uint8_t* data, int timeout){
    if (data == NULL) return 1;

    while (timeout){
        uint8_t status_byte = 0;
        
        int flag = kbc_get_status(&status_byte);
        if (flag) return flag;

        enum kbc_status status = kbc_parse_status(status_byte);
        switch (status){
            case OBF_FULL : {
                --timeout;
                tickdelay(micros_to_ticks(DELAY_US));

                break;
            }
            case NO_ERROR : {
                ++acc;
                return util_sys_inb(KBC_OBF, data);
            }
            default : return 1;
        }
    }

    return 1;
}

int (kbc_get_command)(uint8_t* command){
    if (command == NULL) return 1;

    // notify the KBC that we want to read the command byte
    int flag = sys_outb(KBC_COMMAND_REG, KBC_READ);
    if (flag) return flag;

    return kbc_read_obf(command, 5);
}

int (kbc_write_command)(uint8_t command){
    // notify the KBC that we want to write a new command byte
    int flag = sys_outb(KBC_COMMAND_REG, KBC_WRITE);
    if (flag) return flag;

    return sys_outb(KBC_OBF, command);
}

int (kbc_enable_int)(){
    uint8_t command = 0;

    // read the command byte
    int flag = kbc_get_command(&command);
    if (flag) return flag;

    // enable interrupts
    command |= KBC_ENABLE_KBD_INT;

    return kbc_write_command(command);
}

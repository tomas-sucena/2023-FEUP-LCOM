#include <lcom/lcf.h>

#include "KBC.h"
#include "i8042.h"

extern uint32_t sysinb_calls;

int (kbc_get_status)(uint8_t* status){
    if (status == NULL) return 1;

    ++sysinb_calls;
    return util_sys_inb(KBC_STATUS_REG, status);
}

struct kbc_status (kbc_parse_status)(uint8_t st){
    struct kbc_status status;

    status.parity_error = (st & KBC_PARITY_ERROR);
    status.timeout_error = (st & KBC_TIMEOUT_ERROR);
    status.mouse_data = (st & KBC_MOUSE_DATA);
    status.ibf_full = (st & KBC_IBF_FULL);
    status.obf_full = (st & KBC_OBF_FULL);

    return status;
}

bool (kbc_can_write)(){
    uint8_t st = 0;

    int flag = kbc_get_status(&st);
    if (flag) return flag;

    return !kbc_parse_status(st).ibf_full;
}

bool (kbc_can_read)(){
    uint8_t st = 0;

    int flag = kbc_get_status(&st);
    if (flag) return flag;

    return kbc_parse_status(st).obf_full;
}

int (kbc_read_obf)(uint8_t* data, int timeout){
    if (data == NULL) return 1;

    while (timeout && !kbc_can_write()){
        --timeout;
        tickdelay(micros_to_ticks(DELAY_US));
    }

    if (!timeout) return 1;

    uint8_t st = 0;
    int flag = kbc_get_status(&st);
    if (flag) return flag;

    ++sysinb_calls;
    flag = util_sys_inb(KBC_OBF, data);
    if (flag) return flag;

    struct kbc_status status = kbc_parse_status(st);
    return (status.parity_error || status.timeout_error || status.mouse_data);
}

int (kbc_get_command)(uint8_t* command, int timeout){
    if (command == NULL) return 1;

    int flag;
    while (timeout && !kbc_can_write()){
        --timeout;
        
        flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }

    if (!timeout) return 1;

    // notify the KBC that we want to read the command byte
    flag = sys_outb(KBC_COMMAND_REG, KBC_READ);
    if (flag) return flag;

    ++sysinb_calls;
    return util_sys_inb(KBC_OBF, command);
}

int (kbc_write_command)(uint8_t command, int timeout){
    int flag;

    // notify the KBC that we want to write a new command byte
    int temp = timeout;
    while (timeout && !kbc_can_write()){
        --timeout;
        
        flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }
        
    if (!timeout) return 1;

    flag = sys_outb(KBC_COMMAND_REG, KBC_WRITE);
    if (flag) return flag;

    // write the new command byte
    timeout = temp;
    while (timeout && !kbc_can_write()){
        --timeout;
        
        flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }
        
    if (!timeout) return 1;

    return sys_outb(KBC_IBF, command);
}

int (kbc_enable_int)(){
    uint8_t command = 0;

    // read the command byte
    int flag = kbc_get_command(&command, 5);
    if (flag) return flag;

    // enable interrupts
    command |= KBC_ENABLE_KBD_INT;

    return kbc_write_command(command, 5);
}

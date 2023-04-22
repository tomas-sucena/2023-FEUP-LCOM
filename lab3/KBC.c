#include <lcom/lcf.h>

#include "KBC.h"

extern uint32_t sysinb_calls;

int (kbc_get_status)(){
    ++sysinb_calls;
    return util_sys_inb(KBC_STATUS_REG, &st);
}

kbc_status (kbc_parse_status)(){
    kbc_status status;

    status.parity_error = (st & KBC_PARITY_ERROR);
    status.timeout_error = (st & KBC_TIMEOUT_ERROR);
    status.mouse_data = (st & KBC_MOUSE_DATA);
    status.ibf_full = (st & KBC_IBF);
    status.obf_full = (st & KBC_OBF);

    return status;
}

bool (kbc_can_write)(){
    int flag = kbc_get_status();
    if (flag) return flag;

    // check if the IBF is not full
    return !(st & KBC_IBF);
}

bool (kbc_can_read)(){
    int flag = kbc_get_status();
    if (flag) return flag;

    // check if the OBF is full
    return (st & KBC_OBF);
}

int (kbc_delay_write)(uint32_t wait_ticks){
    while (wait_ticks && !kbc_can_write()){
        --wait_ticks;
        
        int flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }

    return !wait_ticks;
}

int (kbc_delay_read)(uint32_t wait_ticks){
    while (wait_ticks && !kbc_can_read()){
        --wait_ticks;

        int flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }

    return !wait_ticks;
}

int (kbc_read_out_buf)(uint8_t* data, uint32_t wait_ticks){
    if (data == NULL) return 1;

    int flag = kbc_delay_read(wait_ticks);
    if (flag) return flag;

    ++sysinb_calls;
    return util_sys_inb(KBC_OUT_BUF, data);
}

int (kbc_write_in_buf)(uint8_t data, uint32_t wait_ticks){
    int flag = kbc_delay_write(wait_ticks);
    if (flag) return flag;

    return sys_outb(KBC_IN_BUF, data);
}

int (kbc_write_command)(uint8_t command, uint32_t wait_ticks){
    int flag = kbc_delay_write(wait_ticks);
    if (flag) return flag;

    return sys_outb(KBC_COMMAND_REG, command);
}

int (kbc_get_command_byte)(uint8_t* command, uint32_t wait_ticks){
    if (command == NULL) return 1;

    // notify the KBC that we want to read the command byte
    int flag = kbc_write_command(KBC_GET_COMMAND, wait_ticks);
    if (flag) return flag;

    // read the command byte
    return kbc_read_out_buf(command, wait_ticks);
}

int (kbc_set_command_byte)(uint8_t command, uint32_t wait_ticks){
    // notify the KBC that we want to write a new command byte
    int flag = kbc_write_command(KBC_SET_COMMAND, wait_ticks);
    if (flag) return flag;

    // write the new command byte
    return kbc_write_in_buf(command, wait_ticks);
}

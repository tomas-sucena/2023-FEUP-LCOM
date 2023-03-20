#include <lcom/lcf.h>

#include "KBC.h"
#include "i8042.h"

extern uint32_t sysinb_calls;

int (kbc_get_status)(){
    ++sysinb_calls;
    return util_sys_inb(KBC_STATUS_REG, &st);
}

struct kbc_status (kbc_parse_status)(){
    struct kbc_status status;

    status.parity_error = (st & KBC_PARITY_ERROR);
    status.timeout_error = (st & KBC_TIMEOUT_ERROR);
    status.mouse_data = (st & KBC_MOUSE_DATA);
    status.ibf_full = (st & KBC_IBF_FULL);
    status.obf_full = (st & KBC_OBF_FULL);

    return status;
}

bool (kbc_can_write)(){
    int flag = kbc_get_status();
    if (flag) return flag;

    return !kbc_parse_status().ibf_full;
}

bool (kbc_can_read)(){
    int flag = kbc_get_status();
    if (flag) return flag;

    return kbc_parse_status().obf_full;
}

int (kbc_delay_write)(int wait_seconds){
    wait_seconds *= 60;

    while (wait_seconds && !kbc_can_write()){
        --wait_seconds;
        
        int flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }

    return !wait_seconds;
}

int (kbc_delay_read)(int wait_seconds){
    wait_seconds *= 60;
    
    while (wait_seconds && !kbc_can_read()){
        --wait_seconds;

        int flag = tickdelay(micros_to_ticks(DELAY_US));
        if (flag) return flag;
    }

    return !wait_seconds;
}

int (kbc_read_obf)(uint8_t* data, int wait_seconds){
    if (data == NULL) return 1;

    int flag = kbc_delay_read(wait_seconds);
    if (flag) return flag;

    ++sysinb_calls;
    return util_sys_inb(KBC_OBF, data);
}

int (kbc_write_command)(uint8_t command, int wait_seconds){
    int flag = kbc_delay_write(wait_seconds);
    if (flag) return flag;

    return sys_outb(KBC_COMMAND_REG, command);
}

int (kbc_get_command_byte)(uint8_t* command, int wait_seconds){
    if (command == NULL) return 1;

    // notify the KBC that we want to read the command byte
    int flag = kbc_write_command(KBC_READ, wait_seconds);
    if (flag) return flag;

    // read the command byte
    flag = kbc_delay_read(wait_seconds);
    if (flag) return flag;

    return kbc_read_obf(command, wait_seconds);
}

int (kbc_set_command_byte)(uint8_t command, int wait_seconds){
    // notify the KBC that we want to write a new command byte
    int flag = kbc_delay_write(wait_seconds);
    if (flag) return flag;

    flag = sys_outb(KBC_COMMAND_REG, KBC_WRITE);
    if (flag) return flag;

    // write the new command byte
    flag = kbc_delay_write(wait_seconds);
    if (flag) return flag;

    return sys_outb(KBC_IBF, command);
}

int (kbc_enable_int)(int wait_seconds){
    uint8_t command = 0;

    // read the command byte
    int flag = kbc_get_command_byte(&command, 5);
    if (flag) return flag;

    // enable interrupts
    command |= KBC_ENABLE_KBD_INT;
    return kbc_set_command_byte(command, 5);
}

int (kbc_get_mouse_byte)(uint8_t* byte, int wait_seconds){
    return 0; 
}

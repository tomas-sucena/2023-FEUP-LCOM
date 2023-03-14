#include <lcom/lcf.h>

#include "KBC.h"

int (kbc_get_command)(uint8_t* command){
    if (command == NULL) return 1;

    // notify the KBC that we want to read the command byte
    int flag = sys_outb(KBD_COMMAND_REG, KBC_READ);
    if (flag) return flag;

    return util_sys_inb(KBD_OBF, command);
}

int (kbc_write_command)(uint8_t command){
    // notify the KBC that we want to write a new command byte
    int flag = sys_outb(KBD_COMMAND_REG, KBC_WRITE);
    if (flag) return flag;

    return sys_outb(KBD_OBF, command);
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

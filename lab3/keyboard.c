#include <lcom/lcf.h>

#include "keyboard.h"

#include <stdbool.h>

extern int hook_id;
extern uint32_t cnt;
extern bool valid_data;

int (kbd_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}

void (kbd_get_scancode)(uint8_t* scancode){
    uint8_t st = 0;

    kbd_get_status(&st);
    valid_data = (kbd_parse_status(st) == NO_ERROR); 

    util_sys_inb(KBD_OBF, scancode); // even if the data is invalid, we must read it
    ++cnt;
}

int (kbd_get_status)(uint8_t* st){
    if (st == NULL) return 1;
    return util_sys_inb(KBD_STATUS_REG, st);
}

enum kbd_status (kbd_parse_status)(uint8_t st){
    if (st & KBD_PARITY_ERROR)
        return PARITY_ERROR;

    if (st & KBD_TIMEOUT_ERROR)
        return TIMEOUT_ERROR;

    if (st & KBD_MOUSE_DATA)
        return MOUSE_DATA;

    if (!(st & KBD_OBF_FULL))
        return OBF_FULL;

    return NO_ERROR;
}

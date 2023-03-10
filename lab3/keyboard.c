#include <lcom/lcf.h>

#include "keyboard.h"

#include <stdbool.h>

extern int hook_id;
extern uint8_t scancode;
extern uint32_t cnt;
extern bool valid_scancode;

int (kbd_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}

void (kbd_ih)(){
    uint8_t st = 0;

    kbd_read_status(&st);
    valid_scancode = kbd_parse_status(st); 

    util_sys_inb(KBD_OBF, &scancode); // even if the data is invalid, we must read it
    ++cnt;
}

int (kbd_read_status)(uint8_t* st){
    if (st == NULL) return 1;
    return util_sys_inb(KBD_STATUS_REG, st);
}

int (kbd_parse_status)(uint8_t st){
    if (!(st & KBD_OBF_FULL)) return 1;
    return (st & (KBD_PARITY_ERR | KBD_TIMEOUT_ERR | KBD_MOUSE_DATA));
}

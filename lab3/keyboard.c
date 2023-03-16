#include <lcom/lcf.h>

#include <stdbool.h>

#include "keyboard.h"
#include "KBC.h"

extern int hook_id;
extern bool valid_data;

int (kbd_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&hook_id);
}

int (kbd_enable_int)(){
    return kbc_enable_int();
}

void (kbd_get_scancode)(uint8_t* scancode){
    valid_data = !kbc_read_obf(scancode, 5);    
}

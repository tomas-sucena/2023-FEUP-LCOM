#include <lcom/lcf.h>

#include "keyboard.h"
#include "KBC.h"

extern int kbd_hook_id;

int (kbd_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = kbd_hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_ENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&kbd_hook_id);
}

int (kbd_enable_int)(){
    return kbc_enable_int();
}

void (kbd_get_scancode)(struct obf_data* data){
    data->valid = !kbc_read_obf(&data->scancode, 5);
    data->two_byte = (data->scancode == KBD_2B_SCANCODE);    
}

#include <lcom/lcf.h>

#include "keyboard.h"

extern int kbd_hook_id;
extern bool ih_error;

int (kbd_enable_int)(uint32_t wait_ticks){
    return kbc_enable_kbd_int(wait_ticks);
}

int (kbd_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = kbd_hook_id;
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &kbd_hook_id);
}

int (kbd_unsubscribe_int)(){
    return sys_irqrmpolicy(&kbd_hook_id);
}

void (kbd_get_scancode)(struct kbd_data* data, uint32_t wait_ticks){
    ih_error = kbc_read_out_buf(&data->scancode, wait_ticks);
    if (ih_error) return;

    struct kbc_status status = kbc_parse_status();

    data->valid = !(status.parity_error || status.timeout_error || status.mouse_data);
    data->two_byte = (data->scancode == KBD_2B_SCANCODE);    
}

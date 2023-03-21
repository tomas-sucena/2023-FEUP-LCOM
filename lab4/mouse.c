#include <lcom/lcf.h>

#include "mouse.h"
#include "KBC.h"

extern int mouse_hook_id;
extern struct packet pp;
extern uint8_t counter;

int (mouse_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = mouse_hook_id;
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

int (mouse_enable_stream_mode)(int wait_ticks){
    uint8_t ack = 0;
    while (ack != MOUSE_SUCCESS_BYTE){
        int flag = kbc_write_command(KBC_FORWARD_TO_MOUSE, wait_ticks);
        if (flag) return flag;

        flag = kbc_write_ibf(KBC_ENABLE, wait_ticks);
        if (flag) return flag;

        // read the acknowledgement byte
        flag = kbc_read_obf(&ack, wait_ticks);
        if (flag) return flag;
    }

    return 0;
}

int (mouse_disable_stream_mode)(int wait_ticks){
    uint8_t ack = 0;
    while (ack != MOUSE_SUCCESS_BYTE){
        int flag = kbc_write_command(KBC_FORWARD_TO_MOUSE, wait_ticks);
        if (flag) return flag;

        flag = kbc_write_ibf(KBC_DISABLE, wait_ticks);
        if (flag) return flag;

        // read the acknowledgement byte
        flag = kbc_read_obf(&ack, wait_ticks);
        if (flag) return flag;
    }

    return 0;
}

void (mouse_get_data)(struct packet* pp, int wait_ticks){
    uint8_t data = 0;
    kbc_read_obf(&data, wait_ticks);

    // check if there were any errors
    struct kbc_status status = kbc_parse_status();
    if (status.parity_error || status.timeout_error || !status.mouse_data){
        counter = 0;
        return;
    }

    if (counter || is_first_byte(data))
        pp->bytes[counter++] = data;
}

void mouse_parse_packet(struct packet* pp){
    uint8_t first_byte = pp->bytes[0];
    uint8_t delta_x = pp->bytes[1], delta_y = pp->bytes[2];

    // overflow
    pp->y_ov = (first_byte & MOUSE_Y_OVFL);
    pp->x_ov = (first_byte & MOUSE_X_OVFL);

    // offsets
    pp->delta_y = (first_byte & MOUSE_Y_MSB) ? sign_extend(delta_y) : delta_y;
    pp->delta_x = (first_byte & MOUSE_X_MSB) ? sign_extend(delta_x) : delta_x;

    // buttons
    pp->mb = (first_byte & MOUSE_MIDDLE_BUTTON);
    pp->rb = (first_byte & MOUSE_RIGHT_BUTTON);
    pp->lb = (first_byte & MOUSE_LEFT_BUTTON);
}

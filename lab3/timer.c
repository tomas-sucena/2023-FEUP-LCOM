#include "timer.h"

extern int timer_hook_id;
extern uint32_t ticks;

int (timer_write_command)(uint8_t command){
    return sys_outb(TIMER_CTRL, command);
}

int (timer_make_control_word)(uint8_t timer, enum timer_init init_mode, uint8_t *control_word){
    if (timer > 2 || control_word == NULL) return 1;

    // get timer status
    uint8_t st = 0;

    int flag = timer_get_status(timer, &st);
    if (flag) return flag;

    st &= 0xF; // least significant bits

    // get the counter initialization bits
    uint8_t count_init = 0;
    switch (init_mode){
        case LSB_only : {
            count_init = TIMER_LSB;
            break;
        }
        case MSB_only : {
            count_init = TIMER_MSB;
            break;
        }
        case MSB_after_LSB : {
            count_init = TIMER_LSB_MSB;
            break;
        }
        default : return 1;
    }

    // make the control word
    *control_word = TIMER_SEL(timer) | count_init | st;
    return 0;
}

int (timer_make_read_back)(uint8_t timer, uint8_t *read_back){
    if (timer > 2 || read_back == NULL) return 1;

    *read_back = TIMER_RB | TIMER_RB_COUNT | TIMER_RB_SEL(timer);
    return 0;
}

int (timer_get_status)(uint8_t timer, uint8_t *st){
    if (timer > 2 || st == NULL) return 1;

    // make Read-Back command
    uint8_t read_back = 0;

    int flag = timer_make_read_back(timer, &read_back);
    if (flag) return flag;

    // output Read-Back command
    flag = timer_write_command(read_back);
    if (flag) return flag;

    // read status byte
    return util_sys_inb(TIMER(timer), st);
}

timer_status (timer_parse_status)(uint8_t st){
    timer_status status;

    status.timer = get_timer(st);

    // parse the counter initialization mode
    switch (get_count_init(st)){
        case 1 : {
            status.init_mode = LSB_only;
            break;
        }
        case 2 : {
            status.init_mode = MSB_only;
            break;
        }
        case 3 : {
            status.init_mode = MSB_after_LSB;
            break;
        }
        default : {
            status.init_mode = INVAL_val;
            break;
        }
    }

    // parse the counting mode
    uint8_t count_mode = get_count_mode(st);
    if (count_mode > 5)
        count_mode &= ~BIT(2);

    status.count_mode = count_mode;

    // parse the base
    status.bcd = is_BCD(st);

    return status;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq){
    if (timer > 2 || !is_valid_frequency(freq))
        return 1;

    // make the control word
    uint8_t control_word = 0;

    int flag = timer_make_control_word(timer, MSB_after_LSB, &control_word);
    if (flag) return flag;

    // output the control word
    flag = timer_write_command(control_word);
    if (flag) return flag;

    // calculate the initial counter value
    uint16_t initial_counter = (uint16_t) (TIMER_FREQ / freq);

    // output the LSB
    uint8_t lsb = 0, msb = 0;

    flag = util_get_LSB(initial_counter, &lsb);
    if (flag) return flag;
    
    flag = sys_outb(TIMER(timer), lsb);
    if (flag) return flag;

    // output the MSB
    flag = util_get_MSB(initial_counter, &msb);
    if (flag) return flag;
    
    return sys_outb(TIMER(timer), msb);
}

int (timer_subscribe_int)(uint8_t* bit_no){
    if (bit_no == NULL) return 1;

    *bit_no = timer_hook_id;
    return sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id);
}

int (timer_unsubscribe_int)(){
    return sys_irqrmpolicy(&timer_hook_id);
}

void (timer_int_handler)(){
    --ticks;
}

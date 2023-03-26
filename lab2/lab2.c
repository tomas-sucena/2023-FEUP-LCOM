#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

#include "timer.h"

uint32_t timer_hook_id;
uint32_t ticks;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab2/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
    uint8_t st = 0;

    // get the configuration
    int flag = timer_get_status(timer, &st);
    if (flag) return flag;

    // parse the configuration
    union timer_status_field_val config;
    switch (field){
        case tsf_all : {
            config.byte = st;
            break;
        }
        case tsf_initial : {
            config.in_mode = timer_parse_status(st).init_mode;
            break;
        }
        case tsf_mode : {
            config.count_mode = timer_parse_status(st).count_mode;
            break;
        }
        case tsf_base : {
            config.bcd = timer_parse_status(st).bcd;
            break;
        }
    }

    // display the configuration
    return timer_print_config(timer, field, config);
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
    return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
    // global variables
    timer_hook_id = 0;
    ticks = time * sys_hz();
    
    // local variables
    int ipc_status = 0;
    message msg;

    uint8_t timer_bit_no = 0;
    int flag = timer_subscribe_int(&timer_bit_no);
    if (flag) return flag;

    uint32_t timer_mask = BIT(timer_bit_no);

    // interrupt loop
    while (ticks){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool timerInt = msg.m_notify.interrupts & timer_mask;
                if (!timerInt) break;

                timer_int_handler();
                if (ticks % sys_hz()) break;
                
                flag = timer_print_elapsed_time();
                if (flag) return flag;
            }
            default : break;
        }
    }

    return timer_unsubscribe_int();
}

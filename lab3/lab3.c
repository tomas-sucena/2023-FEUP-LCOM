#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "timer.h"

#define WAIT 5

int kbd_hook_id, timer_hook_id;
uint32_t sysinb_calls, ticks;
bool ih_error;
kbd_data data;

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need it]
    lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab3/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
    return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

void (kbd_ih)(){
    kbd_get_scancode(&data, WAIT);
}

int(kbd_test_scan)() {
    // global variables
    kbd_hook_id = 0;
    sysinb_calls = 0;
    
    // local variables
    int ipc_status;
    message msg;
    uint8_t bit_no = 0;
    uint32_t mask = BIT(bit_no);

    int flag = kbd_subscribe_int(&bit_no);
    if (flag) return flag;

    uint8_t scancode[2];
    uint8_t index = 0;
    
    while (data.scancode != KBD_ESC_BREAKCODE){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool subscribedInt = msg.m_notify.interrupts & mask;
                if (!subscribedInt) break;

                kbd_ih();

                if (ih_error) return ih_error;
                if (!data.valid) break;

                scancode[index] = data.scancode;
                if (data.two_byte){
                    ++index;
                    break;
                }

                kbd_print_scancode(is_makecode(scancode[index]), index + 1, scancode);
                index = 0;
            }
            default : break;
        }
    }

    flag = kbd_unsubscribe_int();
    if (flag) return flag;

    return kbd_print_no_sysinb(sysinb_calls);
}

int(kbd_test_poll)() {
    // global variables
    sysinb_calls = 0;

    // local variables
    uint8_t scancode[2];
    uint8_t index = 0;

    while (data.scancode != KBD_ESC_BREAKCODE){
        kbd_ih();

        if (ih_error) return ih_error;
        if (!data.valid) break;

        scancode[index] = data.scancode;
        if (data.two_byte){
            continue;
            ++index;
        }

        kbd_print_scancode(is_makecode(scancode[index]), index + 1, scancode);
        index = 0;
    }

    int flag = kbd_enable_int(WAIT);
    if (flag) return flag;

    return kbd_print_no_sysinb(sysinb_calls);
}

int(kbd_test_timed_scan)(uint8_t idle) {
    // global variables
    timer_hook_id = 0;
    kbd_hook_id = 1;
    sysinb_calls = 0;
    ticks = idle * 60;

    // local variables
    uint8_t timer_bit_no = 0;
    int flag = timer_subscribe_int(&timer_bit_no);
    if (flag) return flag;

    uint8_t kbd_bit_no = 0;
    flag = kbd_subscribe_int(&kbd_bit_no);
    if (flag) return flag;

    uint32_t timer_mask = BIT(timer_bit_no);
    uint32_t kbd_mask = BIT(kbd_bit_no);

    int ipc_status;
    message msg;
    uint8_t scancode[2];
    uint8_t index = 0;
    
    while (data.scancode != KBD_ESC_BREAKCODE && ticks){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool timerInt = msg.m_notify.interrupts & timer_mask;
                if (timerInt) timer_int_handler();

                bool kbdInt = msg.m_notify.interrupts & kbd_mask;
                if (!kbdInt) break;

                kbd_ih();

                if (ih_error) return ih_error;
                if (!data.valid) break;

                scancode[index] = data.scancode;
                if (data.two_byte){
                    ++index;
                    break;
                }

                kbd_print_scancode(is_makecode(scancode[index]), index + 1, scancode);
                index = 0;
                ticks = idle * 60;
            }
            default : break;
        }
    }

    flag = timer_unsubscribe_int();
    if (flag) return flag;

    flag = kbd_unsubscribe_int();
    if (flag) return flag;

    return kbd_print_no_sysinb(sysinb_calls);
}

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"
#include "timer.h"

#define WAIT 5

int mouse_hook_id, timer_hook_id;
uint32_t ticks;
bool ih_error;
struct packet pp;
uint8_t counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

void (mouse_ih)(){
    mouse_get_data(&pp, WAIT);
}

int (mouse_test_packet)(uint32_t cnt) {
    // global variables
    mouse_hook_id = 0;
    counter = 0;

    // local variables
    int ipc_status;
    message msg;
    uint8_t mouse_bit_no = 0;
    uint32_t mask = BIT(mouse_bit_no);

    int flag = mouse_enable_data_report(WAIT);
    if (flag) return flag;

    flag = mouse_subscribe_int(&mouse_bit_no);
    if (flag) return flag;

    while (cnt){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool subscribedInt = msg.m_notify.interrupts & mask;
                if (!subscribedInt) break;

                mouse_ih();

                if (ih_error) return ih_error;
                if (counter < 3) break;

                mouse_parse_packet(&pp);        
                mouse_print_packet(&pp);

                counter = 0;
                --cnt;
            }
            default : break;
        }
    }

    flag = mouse_unsubscribe_int();
    if (flag) return flag;

    return mouse_disable_data_report(WAIT);
}

int (mouse_test_async)(uint8_t idle_time) {
    // global variables
    mouse_hook_id = 0;
    timer_hook_id = 1;
    counter = 0;
    ticks = idle_time * 60;

    // local variables
    int ipc_status;
    message msg;
    uint8_t mouse_bit_no = 0, timer_bit_no = 0;

    int flag = mouse_enable_data_report(WAIT);
    if (flag) return flag;

    flag = mouse_subscribe_int(&mouse_bit_no);
    if (flag) return flag;

    flag = timer_subscribe_int(&timer_bit_no);
    if (flag){
        mouse_unsubscribe_int();
        return flag;
    }

    uint32_t mouse_mask = BIT(mouse_bit_no);
    uint32_t timer_mask = BIT(timer_bit_no);

    while (ticks){
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

                bool mouseInt = msg.m_notify.interrupts & mouse_mask;
                if (!mouseInt) break;

                mouse_ih();

                if (ih_error) return ih_error;
                if (counter < 3) break;

                mouse_parse_packet(&pp);
                mouse_print_packet(&pp);

                counter = 0;
                ticks = idle_time * 60;
            }
            default : break;
        }
    }    
    
    flag = timer_unsubscribe_int();
    if (flag) return flag;    
    
    flag = mouse_unsubscribe_int();
    if (flag) return flag;

    return mouse_disable_data_report(WAIT);
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

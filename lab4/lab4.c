// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "mouse.h"

#define WAIT 5

int mouse_hook_id;
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

    // local variables
    int ipc_status;
    message msg;
    uint8_t mouse_bit_no = 0;
    uint32_t mask = BIT(mouse_bit_no);

    int flag = mouse_subscribe_int(&mouse_bit_no);
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
                if (counter < 3) break;

                --cnt;
                flag = mouse_print_packet(&pp);
                counter = 0;
            }
            default : break;
        }
    }

    return mouse_unsubscribe_int();
}

int (mouse_test_async)(uint8_t idle_time) {
    /* To be completed */
    printf("%s(%u): under construction\n", __func__, idle_time);
    return 1;
}

int (mouse_test_gesture)() {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

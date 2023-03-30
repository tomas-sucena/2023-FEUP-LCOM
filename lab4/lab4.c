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

/* MOUSE GESTURE STATE MACHINE */
#define LEFT 0
#define RIGHT 1

enum logical_and_gesture {
    START, // 0
    RB_PRESSED, // 1
    LEFT_LINE_DRAW, // 2
    RB_RELEASED, // 3
    LB_PRESSED, // 4
    RIGHT_LINE_DRAW, // 5
    LB_RELEASED // 6
};

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

void process_state(uint8_t tolerance, uin16_t* x_vals, uint16_t* y_vals, uintenum logical_and_gesture* state){
    uint16_t x = pp.delta_x, y = pp.delta_y;

    bool slight_movement_x = (abs(x) <= tolerance);
    bool slight_movement_y = (abs(y) <= tolerance);
    bool slight_movement = (slight_movement_x && slight_movement_y);

    switch (state){
        case START : {
            if (pp.rb && !pp.lb && !pp.mb) state = RB_PRESSED;
            return;
        }
        case RB_PRESSED: {
            if (!pp.rb || pp.lb || pp.mb || !slight_movement)
                break;

            x_vals[LEFT] += x;
            y_vals[LEFT] += y;

            state = LEFT_LINE_DRAW;
            return;
        }       
        case LEFT_LINE_DRAW: {
            if (pp.lb || pp.mb || (y < 0 && !slight_movement_y))
                break;

            if (!pp.rb) state = RB_RELEASED;
            
            x_vals[LEFT] += x;
            y_vals[LEFT] += y;

            return;
        }
        case RB_RELEASED: {
            if (pp.mb || pp.rb || !slight_movement)
                break;

            if (pp.lb) state = LB_PRESSED;

            x_vals[LEFT] += x;
            y_vals[LEFT] += y;

            return;
        }
        case LB_PRESSED: {
            if (!pp.lb || pp.rb || pp.mb || (y > 0 && !slight_movement_y))
                break;

            x_vals[RIGHT] += x;
            y_vals[RIGHT] += y;

            return;
        }
        case RIGHT_LINE_DRAW: {
            if (pp.rb || pp.mb || (y > 0 && !slight_movement_y))
                break;

            if (!pp.lb) state = LB_RELEASED;
            
            x_vals[RIGHT] += x;
            y_vals[RIGHT] += y;
            
            return;
        }
        default : return;     
    }

    state = START;
    x_vals[LEFT] = x_vals[RIGHT] = 0;
    y_vals[LEFT] = y_vals[RIGHT] = 0;
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
    // global variables
    mouse_hook_id = 0;
    counter = 0;

    // local variables
    int ipc_status;
    message msg;
    uint8_t mouse_bit_no = 0;

    int flag = mouse_enable_data_report(WAIT);
    if (flag) return flag;

    flag = mouse_subscribe_int(&mouse_bit_no);
    if (flag) return flag;

    uint32_t mouse_mask = BIT(mouse_bit_no);

    enum logical_and_gesture state = START;
    uint32_t x_total = 0;

    while (state != LB_RELEASED){
        flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch (_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool mouseInt = msg.m_notify.interrupts & mouse_mask;
                if (!mouseInt) break;

                mouse_ih();
                if (counter < 3) break;

                mouse_parse_packet(&pp);
                process_state(x_len, tolerance, &x_total, &state);
            }
            default : break;
        }
    }

    flag = mouse_unsubscribe_int();
    if (flag) return flag;

    return mouse_disable_data_report(WAIT);
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    /* This year you need not implement this. */
    printf("%s(%u, %u): under construction\n", __func__, period, cnt);
    return 1;
}

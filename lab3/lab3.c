#include <lcom/lcf.h>
#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"

int hook_id;
uint32_t cnt;
uint8_t data;
bool valid_data;

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

void kbd_ih(){
    data = 0;
    kbd_get_scancode(&data);
}

int(kbd_test_scan)() {
    // global variabes
    hook_id = 0;
    cnt = 0;
    
    // local variables
    int ipc_status;
    message msg;
    uint8_t bit_no = 0;
    uint8_t mask = BIT(bit_no);

    int flag = kbd_subscribe_int(&bit_no);
    if (flag) return flag;

    uint8_t scancode[2];
    bool two_byte_scancode = false;
    
    while (data != KBD_ESC_BREAKCODE){
        int flag = driver_receive(ANY, &msg, &ipc_status);
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
                if (!valid_data) break;

                if (two_byte_scancode){
                    scancode[1] = data;
                    kbd_print_scancode(is_makecode(scancode[1]), 2, scancode);

                    two_byte_scancode = false;

                    break;
                }

                scancode[0] = data;

                two_byte_scancode = (data == KBD_2B_SCANCODE);
                if (two_byte_scancode) break;

                kbd_print_scancode(is_makecode(scancode[0]), 1, scancode);
            }
            default : break;
        }
    }

    flag = kbd_unsubscribe_int();
    if (flag) return flag;

    return kbd_print_no_sysinb(cnt);
}

int(kbd_test_poll)() {
    // global variables
    cnt = 0;

    // local variables
    uint8_t scancode[2];
    bool two_byte_scancode = false;

    while (data != KBD_ESC_BREAKCODE){
        kbd_ih();
        if (!valid_data) continue;

        if (two_byte_scancode){
            scancode[1] = data;
            kbd_print_scancode(is_makecode(scancode[1]), 2, scancode);

            two_byte_scancode = false;

            continue;
        }

        scancode[0] = data;
        
        two_byte_scancode = (data == KBD_2B_SCANCODE);
        if (two_byte_scancode) continue;

        kbd_print_scancode(is_makecode(scancode[0]), 1, scancode);
    }

    int flag = kbd_enable_int();
    if (flag) return flag;

    return kbd_print_no_sysinb(cnt);
}

int(kbd_test_timed_scan)(uint8_t n) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

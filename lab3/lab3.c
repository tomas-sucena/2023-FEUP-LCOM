#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

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

int(kbd_test_scan)() {
    // global variabes
    hook_id = 0;
    acc = 0;
    scancode = 0;

    // local variables
    int ipc_status;
    message msg;
    uint8_t bit_no = 0;

    int flag = kbd_subscribe_int(&bit_no);
    if (flag) return flag;

    while (scancode != KBD_ESC_BREAKCODE){
        int flag = driver_receive(ANY, &msg, &ipc_status);
        if (flag){
            printf("driver_receive failed with: %d", flag);
            continue;
        }

        if (!is_ipc_notify(ipc_status)) continue;

        switch(_ENDPOINT_P(msg.m_source)){
            case HARDWARE : {
                bool subscribedInt = msg.m_notify.interrupts & BIT(bit_no);
                if (!subscribedInt) break;

                kbd_int_handler();

            }
        }
    }

    kbd_unsubscribe_int();
    return 0;
}

int(kbd_test_poll)() {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
    /* To be completed by the students */
    printf("%s is not yet implemented!\n", __func__);

    return 1;
}

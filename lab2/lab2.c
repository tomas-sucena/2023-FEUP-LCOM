#include <lcom/lcf.h>
#include <lcom/lab2.h>

#include <stdbool.h>
#include <stdint.h>

int hook_id;
int acc;

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
  uint8_t status = 0;

  int flag = timer_get_conf(timer, &status);
  if (flag) return flag;

  return timer_display_conf(timer, status, field);
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  return timer_set_frequency(timer, freq);
}

int(timer_test_int)(uint8_t time) {
  // global variables
  hook_id = 0;
  acc = 0;

  // local variables
  int ipc_status;
  message msg;
  uint8_t bit_no = 0;
  uint32_t mask = BIT(bit_no);

  int flag = timer_subscribe_int(&bit_no);
  if (flag) return flag;

  while (acc < time * 60){
    int flag = driver_receive(ANY, &msg, &ipc_status);
    if (flag){
      printf("driver_receive failed with: %d", flag);
      continue;
    }

    if (!is_ipc_notify(ipc_status)) continue;

    switch (_ENDPOINT_P(msg.m_source)){
      case HARDWARE : {
        bool subscribedInt = msg.m_notify.interrupts & mask;
        if (!subscribedInt) break;

        timer_int_handler();
        if (acc % 60) break;

        timer_print_elapsed_time();
      }
      default : break;
    }
  }

  return timer_unsubscribe_int();
}

#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

/* MY FUNCTIONS */
enum timer_init (get_init_mode)(uint8_t status){
  uint8_t mask = BIT(5) | BIT(4);
  uint8_t init_bits = (status & mask) >> 4;

  switch (init_bits){
    case 1 : {
      return LSB_only;
    }
    case 2 : {
      return MSB_only;
    }
    case 3 : {
      return MSB_after_LSB;
    }
  }

  return INVAL_val;
}

uint8_t (get_count_mode)(uint8_t status){
  uint8_t mask = BIT(3) | BIT(2) | BIT(1);
  uint8_t count_bits = (status & mask) >> 1;

  if (count_bits > 5)
    count_bits &= ~BIT(2);

  return count_bits;
}

bool (is_bcd)(uint8_t status){
  return status & BIT(0);
}

int (make_control_word)(uint8_t timer, enum timer_init init, uint8_t* control_word){
  if (timer > 2) return 1;

  // read the timer's configuration
  uint8_t status = 0;

  int flag = timer_get_conf(timer, &status);
  if (flag) return flag;

  status &= 0xF; // least significant bits

  // create the control word
  *control_word = status;

  switch (timer) {
    case 0 : {
      *control_word |= TIMER_SEL0;
      break;
    }
    case 1 : {
      *control_word |= TIMER_SEL1;
      break;
    }
    case 2 : {
      *control_word |= TIMER_SEL2;
      break;
    }
  }

  switch (init){
    case LSB_only : {
      *control_word |= TIMER_LSB;
      break;
    }
    case MSB_only : {
      *control_word |= TIMER_MSB;
      break;
    }
    case MSB_after_LSB : {
      *control_word |= TIMER_LSB_MSB;
      break;
    }
    default : return 1;
  }

  return 0;
}

/* LAB FUNCTIONS */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer > 2) return 1;

  // create the control word
  uint8_t control_word = 0;

  int flag = make_control_word(timer, MSB_after_LSB, &control_word);
  if (flag) return flag;
  
  // output the control word
  flag = sys_outb(TIMER_CTRL, control_word);
  if (flag) return flag;

  // output initial counting value
  uint16_t initial_count = (uint16_t) (TIMER_FREQ / freq);

  uint8_t lsb = 0, msb = 0;
  if (util_get_LSB(initial_count, &lsb) || util_get_MSB(initial_count, &msb))
    return 1;

  flag = sys_outb(TIMER(timer), lsb);
  if (flag) return flag;

  flag = sys_outb(TIMER(timer), msb);
  if (flag) return flag;

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer > 2) return 1;
  
  // create Read-Back command
  uint8_t read_back = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  
  // output Read-Back command
  int flag = sys_outb(TIMER_CTRL, read_back);
  if (flag) return flag;

  // read timer configuration
  return util_sys_inb(TIMER(timer), st);
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  if (timer > 2) return 1;
  
  union timer_status_field_val conf;
  switch (field){
    case tsf_all : {
      conf.byte = st;
      break;
    }
    case tsf_initial : {
      conf.in_mode = get_init_mode(st);
      break;
    }
    case tsf_mode : {
      conf.count_mode = get_count_mode(st);
      break;
    }
    case tsf_base : {
      conf.bcd = is_bcd(st);
      break;
    }
  }

  return timer_print_config(timer, field, conf);
}

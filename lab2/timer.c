#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

/* MY FUNCTIONS */
enum timer_init (get_init_mode)(uint8_t status){
  uint8_t mask = BIT(5) | BIT(4);
  uint8_t init_bits = (status | mask) >> 4;

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

uint8_t get_count_mode(uint8_t status){
  uint8_t mask = BIT(3) | BIT(2) | BIT(1);
  uint8_t count_bits = (status | mask) >> 1;

  if (count_bits > 5)
    count_bits &= ~BIT(2);

  return count_bits;
}

bool is_bcd(uint8_t status){
  return status & BIT(0);
}

/* LAB FUNCTIONS */
int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
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
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

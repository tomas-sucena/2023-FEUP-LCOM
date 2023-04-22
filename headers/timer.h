#ifndef _LCOM_TIMER_H_
#define _LCOM_TIMER_H_

#include <lcom/lcf.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8254.h"

/* IRQ */
#define TIMER0_IRQ 0

typedef struct {
    uint8_t timer;
    enum timer_init init_mode;
    uint8_t count_mode;
    bool bcd;
} timer_status;

int (timer_write_command)(uint8_t command);
int (timer_make_control_word)(uint8_t timer, enum timer_init init_mode, uint8_t *control_word);
int (timer_make_read_back)(uint8_t timer, uint8_t *read_back);
int (timer_get_status)(uint8_t timer, uint8_t *st);
timer_status (timer_parse_status)(uint8_t st);
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

// for interrupts
int (timer_subscribe_int)(uint8_t* bit_no);
int (timer_unsubscribe_int)();
void (timer_int_handler)();

#endif // _LCOM_TIMER_H_

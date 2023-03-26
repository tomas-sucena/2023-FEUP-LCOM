#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

/* FREQUENCY */
#define TIMER_FREQ 1193181
#define is_valid_frequency(freq) (freq >= (TIMER_FREQ / 0xFFFF) && freq <= TIMER_FREQ)

/* I/O PORTS */
#define TIMER(n) (0x40 + n) 
#define TIMER_CTRL 0x43

/* CONTROL WORD BITS */
// counter selection
#define TIMER_SEL(n) (n << 6)
#define get_timer(st) (st & (BIT(7) | BIT(6)) >> 6)

// counter initialization
#define TIMER_LSB BIT(4)
#define TIMER_MSB BIT(5)
#define TIMER_LSB_MSB (TIMER_LSB | TIMER_MSB)
#define get_count_init(st) ((st & TIMER_LSB_MSB) >> 4)

// counting mode
#define get_count_mode(st) ((st & 0xF) >> 1)

// base
#define TIMER_BCD BIT(1)
#define is_BCD(st) (st & BIT(0))

/* READ-BACK COMMAND BITS */
#define TIMER_RB (BIT(6) | BIT(7))
#define TIMER_RB_COUNT BIT(5) // set low
#define TIMER_RB_STATUS BIT(4) // set low
#define TIMER_RB_SEL(n) BIT(n + 1)

#endif // _LCOM_I8254_H_

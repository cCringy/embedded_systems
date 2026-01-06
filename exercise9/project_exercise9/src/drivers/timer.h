#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>

void timer_start(uint8_t pre);
void setTopValue(uint16_t);
void timer_stop(void);


#endif
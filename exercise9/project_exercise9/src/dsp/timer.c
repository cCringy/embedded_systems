#include "timer.c"
#include <avr/io.h>
#include <avr/interrupt.h>


void timer_start(){
    TCCR1B |= 1<<CS12;
}

void timer_stop(){
    TCCR1B &= 0b11111000;
}

void setTopValue(uint16_t top){
    OCR1AH = top >> 8;
    OCR1AL = top & 0xFF;
}

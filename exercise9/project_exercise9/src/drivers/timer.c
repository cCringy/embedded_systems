
#include "timer.c"
#include <avr/io.h>
#include <avr/interrupt.h>


void timer_init(){
    cli();
    TCCR1A = 0; // Lösche potentielle Voreinstellungen (e.g. PWM etc)
    TCCR1B = 0; // Lösche potentielle Voreinstellungen (e.g. PWM etc)

    TCCR1B |= (1<<WGM12);  // Enable CTC Mode

    TIMSK1 |= (1<<OCIE1A); // Enable Interrupts bei Compare Match mit TOP in OCR1A

    OCR1A = 62499-1; // setze TOP
    sei();
}

void timer_start(){
    TCCR1B |= (1<<CS12);
}

void timer_stop(){
    TCCR1B &= 0b11111000;
}
/*
void setTopValue(uint16_t top){
    OCR1AL = top & 0xFF;
    OCR1AH = top >> 8;
}
*/
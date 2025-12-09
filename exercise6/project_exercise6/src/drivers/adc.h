#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#define ADC_PRESCALER_MASK 0x07 // 0b00000111
#define ADC_SET_PRESCALER(p)\
    do {
        ADCSRA = (ADCSRA & ~(ADC_PRESCALER_MASK))|((p) & ADC_PRESCALER_MASK);
    }while(0)// Makro | z.b. ADC_PRESCALER_MASK = 128

void ADC_init(void);
uint16_t ADC_poll(uint8_t channel);

#endif
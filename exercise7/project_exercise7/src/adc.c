#include <avr/io.h>
#include "adc.h"

void ADC_init(void){
    ADMUX |= (1<<REFS0);

    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // Prescaler
    // oder ADC_PRESCALER_MASK = 128

    ADCSRA |= (1 << ADEN);

}



uint8_t adc_read(uint8_t channel){

}
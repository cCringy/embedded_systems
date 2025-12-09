#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC_init(void);
uint16_t ADC_poll(uint8_t channel);

#endif
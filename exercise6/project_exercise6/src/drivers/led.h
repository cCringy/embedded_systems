#ifndef LED_H_
#define LED_H_

#include <avr/io.h>

#define LED_PIN PB5

void LED_init();
void LED_on();
void LED_off();
void LED_toggle();

#endif /* LED_H_ */
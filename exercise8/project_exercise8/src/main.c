#include <stdio.h>
#include <util/delay.h>

#include "debug_uart.h"
#include "drivers/led.h"
#include "drivers/gpio.h"
#include "drivers/adc.h"
#include "dsp/buffer.h"
#include "helpers/debounce.h"

static FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,_FDEV_SETUP_RW);

void adc_interrupt_handler(uint16_t val){
    uint8_t msb=val >> 8;
    uint8_t lsb = val & 0xFF;
    uart_transmit(msb);
    uart_transmit(lsb);
}

void setup(void){
    stdin = stdout = &uart_str;
    uart_init();
    
    ADC_init();
    ADC_set_callback(adc_interrupt_handler);
    ADC_enable_interrupt();
}

int main(void){
    setup();

    while(1){
        LED_toggle();
    }
}
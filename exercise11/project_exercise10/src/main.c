#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "drivers/timer.h"
#include "drivers/adc.h"
#include "debug_uart.h"
#include "dsp/buffer.h"
#include "dsp/timeQueue"
#include "board_config.h"

#define BUFFERSIZE 20
#define N_TAPS     10

volatile buffer_t input_buffer;

void setup(void){
    //timerFreq = 1000 HZ
    // pre = 1 topVal = 16000000/1000*pre = 16.000

    LED_init();

    ADC_init();
    ADC_select_channel(SOUNDSENSOR_CH);
    ADC_set_callback(&handle_adc_value);
    ADC_enable_interrupt();

    timer_init_timer1_pwm();

    input_buffer = buffer_create(BUFFERSIZE);
}

void handle_adc_value(uint16_t value){
    buffer_insert(input_buffer,value);
}

int main(void){
    setup();

    while(1){
        
        if(buffer_peek(input_buffer)!= INVALID_VAL){
            
            uint16_t duty = filter_sound_sensor(input_buffer,N_TAPS);
            OCR1A = (duty > 1023) ? 1023 : duty;
        }
    }
    cli();

    buffer_delete(&input_buffer);
    return 0;
}

ISR(ADC_vect){
    ADC_handle_Interrupt();
}
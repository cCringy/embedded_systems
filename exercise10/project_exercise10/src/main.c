#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "drivers/timer.h"
#include "drivers/adc.h"
#include "debug_uart.h"
#include "dsp/buffer.h"
#include "dsp/timeQueue"
#include "board_config.h"

static FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar,_FDEV_SETUP_RW);
volatile buffer_t input_buffer;

volatile uint16_t currentTime;

void setup(void){
    stdin = stdout = &uart_str;
    uart_init();
    
    ADC_init();
    ADC_select_channel(PHOTORESITOR_CH);
    ADC_set_callback(&insert_into_adcbuffer)
    ADC_enable_interrupt();

    //timerFreq = 1000 HZ
    // pre = 1 topVal = 16000000/1000*pre = 16.000
    timer_init_timer1(1);

    timeq_t q = timeq_create();
}

void task1()
{
    printf("Task1 executed!\n");
}

void task2()
{
    printf("Task2 executed!\n");
}

void task3()
{
    printf("Task3 (periodic) executed!\n");
}

void task4()
{
    printf("Task4 (periodic) executed!\n");
}


int main(void){
    task_t test_task1 = task_createTask(&task1, currentTime, 0, 1000);
    task_t test_task2 = task_createTask(&task2, currentTime, 0, 3500);
    task_t test_task3 = task_createTask(&task3, currentTime, 1000, 3000); // periodic
    task_t test_task4 = task_createTask(&task4, currentTime, 700, 500); // periodic

    setup();

    q.scheduleTask(test_task1);
    q.scheduleTask(test_task1);
    q.scheduleTask(test_task1);
    q.scheduleTask(test_task1);

    uint16_t val;

    while(1){
        if(buffer_getlen(input_buffer)!= 0){
            val = buffer_pop(input_buffer);
            uint8_t msb = val >> 8;
            uint8_t lsb = val & 0xFF;
            uart_transmit(msb);
            uart_transmit(lsb);
        }
    }
    cli();
    buffer_delete((buffer_t*) &input_buffer)
    return 0;
}

ISR(TIMER1_COMPA_vect){
    currentTime++;

}

ISR(ADC_vect){
    ADC_handle_Interrupt();
}


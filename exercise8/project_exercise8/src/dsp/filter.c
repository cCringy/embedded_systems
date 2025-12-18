#include <stdlib.h>
#include "buffer.h"

uint16_t filter_moving_average(buffer_t buffer, uint16_t num_taps){
    uint32_t acc = 0;
    for(int i = 0; i<num_taps;++i){
        acc += buffer_peek(buffer,i);
    }
    return (uint16_t) (acc / num_taps);
}
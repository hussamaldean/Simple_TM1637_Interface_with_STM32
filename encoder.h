#ifndef __encoder__h
#define __encoder__h
#include "stm32f4xx.h"                  // Device header
#include "stdint.h" 
void encoder_init(uint32_t max_value);
uint32_t get_encoder_value(void);


#endif


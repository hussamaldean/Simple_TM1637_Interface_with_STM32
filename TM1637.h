#ifndef __TM1637__h
#define __TM1637__h

#include "stm32f4xx.h"                  // Device header
#include "delay.h"


void tm1637Init(void);
void tm1637DisplayDecimal(int v, int displaySeparator);
void tm1637SetBrightness(char brightness);

#endif


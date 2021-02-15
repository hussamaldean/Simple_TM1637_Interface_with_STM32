#include "delay.h"
volatile uint64_t milli;
volatile uint32_t returnmillis;
volatile int i;
void systick_init(void){
  __disable_irq();

	SysTick->CTRL=0;
	SysTick->LOAD=100000;
	SysTick->VAL=0;
//	
	//SysTick_Config(SystemCoreClock/1000);
	SysTick->CTRL=7; //0b00000111;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;  /*Enable TIM3 clock*/
	TIM1->PSC = 0;  /* 16 000 000 /160 = 100 000*/
	TIM1->ARR = 100;  /* 100 000 /100 = 1000*/
	TIM1->CNT =0;
	
	__enable_irq();	
}


uint32_t millis(void){
//__disable_irq();
	returnmillis=milli;
//__enable_irq();
return returnmillis; 

}


void delay(int ms)
{
int i=millis();
	do
	{;}
	while(millis()-i<ms);
}

void SysTick_Handler(void){
__disable_irq();
milli++;
__enable_irq();
//SysTick->VAL=0;
}

void delayuS(int ms)
{
	TIM1->CR1 =1;
for(i =0;i<ms;i++){
	
		 while(!(TIM1->SR & 1)){;}   /*wait for UIF set*/
			 TIM1->SR &= ~1;
			 
	}
			TIM1->CR1=0; 
	}

	
	
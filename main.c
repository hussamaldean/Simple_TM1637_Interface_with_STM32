#include "encoder.h"
#include "SysClockConfig.h"
#include "TM1637.h"

uint32_t current,previous;
#define rate 1000
int tog=1;
int main(void)

{
SysClockConfig();
systick_init();
encoder_init(9999);
tm1637Init();

while(1)
{

	
	
current=get_encoder_value();

tm1637DisplayDecimal(current,tog);
	
if(millis()-previous>1000){tog=!tog; previous=millis();}

	
}


}



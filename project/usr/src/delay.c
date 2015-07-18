#include "delay.h"

void delay1ms(uint16_t ms)
{
	for(uint16_t j = 0;j < ms;j ++)
	{
		for (uint16_t i = 0;i < 1000;i ++)
		{
			;
		}
	}
}

void delay1s(uint16_t seconds)
{
	for (uint16_t i = 0;i < 1000;i ++)
	{
		delay1ms(1);
	}
}
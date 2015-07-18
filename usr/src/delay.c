#include "delay.h"

void delay1ms(uint8_t ms)
{
	for(uint8_t j = 0;j < ms;j ++)
	{
		for (int i = 0;i < 1000;i ++)
		{
			;
		}
	}
}

void delay1s(uint8_t seconds)
{
	for (int i = 0;i < 1000;i ++)
	{
		delay1ms(1);
	}
}
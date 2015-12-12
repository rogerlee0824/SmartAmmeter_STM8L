#include "Beeper.h"
#include "includes.h"

beeper_event_t beeper_event;
uint8_t beeper_is_running = 0;

static void Beeper_Init(void)
{
	GPIO_Init(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER, GPIO_Mode_Out_PP_Low_Fast);	
}

static void Beeper(uint8_t times)
{
	for(uint8_t i = 0;i < times;i ++)
	{
		GPIO_SetBits(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER);
		delay1ms(50);
		GPIO_ResetBits(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER);
		delay1ms(50);
	}
}

void beeper_event_handler(void * p_event_data, uint16_t event_size)
{
	beeper_event_t * beeper_event_tmp = p_event_data;
	
	switch(beeper_event_tmp->eBeeper_event)
	{
		case BEEPER_INIT:
			Beeper_Init();
			break;

		case BEEPER:
			if(beeper_event_tmp->beeper_times)
			{
				beeper_is_running = 1;
				Beeper(beeper_event_tmp->beeper_times);
				beeper_is_running = 0;
			}
			break;
			
		default:
			break;
	}
}


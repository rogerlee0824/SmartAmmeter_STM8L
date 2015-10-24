#include "Beeper.h"
#include "includes.h"

beeper_event_t beeper_event;
uint8_t beeper_is_running = 0;

static void Beeper_Init(void)
{
	GPIO_Init(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER, GPIO_Mode_Out_PP_Low_Fast);	
}

void Beeper(void)
{
	GPIO_SetBits(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER);
}

void DeBeeper(void)
{
	GPIO_ResetBits(GPIO_PORT_BEEPER, GPIO_PIN_BEEPER);
}

void RTC_Config(void)
{
	CLK_RTCClockConfig(CLK_RTCCLKSource_LSE, CLK_RTCCLKDiv_1);

	/* Enable RTC clock */
  	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);

  	/* Configures the RTC */
  	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
  	RTC_ITConfig(RTC_IT_WUT, ENABLE);

	/* RTC will wake-up from halt every 20second*/
    RTC_SetWakeUpCounter(10);
    RTC_WakeUpCmd(ENABLE);
}

void beeper_event_handler(void * p_event_data, uint16_t event_size)
{
	beeper_event_t * beeper_event_tmp = p_event_data;
	
	switch(beeper_event_tmp->eBeeper_event)
	{
		case BEEPER_INIT:
			Beeper_Init();
			DeBeeper();
			break;

		case BEEPER:
			if(beeper_event_tmp->beeper_times)
			{
				beeper_is_running = 1;
			}
			break;
			
		default:
			break;
	}
}


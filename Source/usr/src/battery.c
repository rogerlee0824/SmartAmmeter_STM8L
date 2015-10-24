#include "battery.h"

battery_event_t battery_event;

void ADC_GPIO_Init(void)
{
	#ifdef BATTERY_DEBUG
		printf("ADC_GPIO_Init\r\n");
	#endif
	
	GPIO_Init(GPIO_PORT_VTEST_5V, GPIO_PIN_VTEST_5V, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_ADC_OUT, GPIO_PIN_ADC_OUT, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_AA_OUT_IN, GPIO_PIN_AA_OUT_IN, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_AA_CTRL, GPIO_PIN_ADC_OUT, GPIO_Mode_Out_PP_Low_Fast);
}

void battery_event_handler(void * p_event_data, uint16_t event_size)
{
	battery_event_t *p_battery_event_tmp = p_event_data;

	switch(p_battery_event_tmp->eBattery_event)
	{
		case ADC_INIT:
			ADC_GPIO_Init();
			break;

		case ADC_SAMPLE:
			
			break;

		default:
			break;
	}
}
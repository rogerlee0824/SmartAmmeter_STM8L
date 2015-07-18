#include "key.h"
#include "includes.h"

key_event_t key_event;

void KEY_Init(void)
{
	/* Disable interrupts */
	disableInterrupts();
	
	#ifdef KEY_DEBUG
		printf("[KEY] KEY_Init...\r\n");
	#endif
	GPIO_Init(GPIO_PORT_KEY, GPIO_PIN_KEY, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_KEY, EXTI_Trigger_KEY);
	
	/* Enable interrupts */
	enableInterrupts();
	EXTI_ClearITPendingBit(EXTI_IT_PIN_KEY);
}

void key_event_handler(void * p_event_data, uint16_t event_size)
{
	key_event_t * key_event_tmp = p_event_data;
	
	switch(key_event_tmp->eKey_event)
	{
		case KEY_INIT:
			KEY_Init();
			break;

		case KEY_HANDLE:
			#ifdef KEY_DEBUG
				printf("[KEY] KEY_HANDLE...\r\n");
			#endif
			break;
			
		default:
			break;
	}
}
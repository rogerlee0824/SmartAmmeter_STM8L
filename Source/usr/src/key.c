#include "key.h"
#include "includes.h"

key_event_t key_event;
uint8_t key_is_pressed = 0;

void KEY_Init(void)
{
	#ifdef KEY_DEBUG
		printf("[KEY] KEY_Init...\r\n");
	#endif
	
	/* Disable interrupts */
	disableInterrupts();
	
	GPIO_Init(GPIO_PORT_KEY, GPIO_PIN_KEY, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_KEY, EXTI_Trigger_KEY);
	
	EXTI_ClearITPendingBit(EXTI_IT_PIN_KEY);
	
	/* Enable interrupts */
	enableInterrupts();
}

/***********************************************************************
  * @brief  Initialise the resource for count.
  * @param  None
  * @retval None
************************************************************************/
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
			if(key_is_pressed)
			{
				lcd_event.eLcd_event = lCD_HANDLE;
            	app_sched_event_put(&lcd_event,sizeof(lcd_event),lcd_event_handler);
				key_is_pressed = 0;
			}
			break;
			
		default:
			break;
	}
}
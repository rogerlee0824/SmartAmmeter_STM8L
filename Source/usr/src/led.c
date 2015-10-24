#include "led.h"
#include "includes.h"

/***********************************************************************
  * @brief  Initializes the GPIO pins for LED
  * @param  None
  * @retval None
************************************************************************/
void LED_Init(void)
{
	GPIO_Init(GPIO_PORT_LED1, GPIO_PIN_LED1, GPIO_Mode_Out_PP_Low_Fast);
}

void LED1_ON(void)
{
	GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

void LED1_OFF(void)
{
	GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

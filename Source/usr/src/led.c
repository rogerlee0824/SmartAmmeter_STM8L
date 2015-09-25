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
	GPIO_Init(GPIO_PORT_LED3, GPIO_PIN_LED3, GPIO_Mode_Out_PP_Low_Fast);
}

void LED1_ON(void)
{
	GPIO_SetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

void LED1_OFF(void)
{
	GPIO_ResetBits(GPIO_PORT_LED1, GPIO_PIN_LED1);
}

void LED3_ON(void)
{
	GPIO_SetBits(GPIO_PORT_LED3, GPIO_PIN_LED3);
}

void LED3_OFF(void)
{
	GPIO_ResetBits(GPIO_PORT_LED3, GPIO_PIN_LED3);
}
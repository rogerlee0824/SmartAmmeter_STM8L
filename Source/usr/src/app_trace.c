#include "app_trace.h"
#include "includes.h"
#include <stdio.h>
#include "stm8l15x_usart.h"
#include "stm8l15x_gpio.h"

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void AppTrace_Init(void)
{
	/* Enables USART3 clock. */
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, ENABLE);
	
#ifndef COUNT_TEST	
	USART_Init(APP_TRACE_USART, 
			   115200, 
			   USART_WordLength_8b, 
			   USART_StopBits_1,
               USART_Parity_No,  
			   USART_Mode_Tx);
#else
	USART_Init(APP_TRACE_USART, 
			   2400, 
			   USART_WordLength_8b, 
			   USART_StopBits_1,
               USART_Parity_No,  
			   USART_Mode_Tx);
#endif
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void AppTrace_DeInit(void)
{
	/* Disables USART3 clock. */
	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, DISABLE);
	
	USART_DeInit(APP_TRACE_USART);
	GPIO_Init(GPIO_PORT_USART_TX, GPIO_PIN_USART_TX, GPIO_Mode_Out_PP_Low_Fast);
	//GPIO_Init(GPIO_PORT_USART_RX, GPIO_PIN_USART_RX, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
int putchar(int ch)
{
	uint16_t time_out = 10000;
	
	USART_SendData8(APP_TRACE_USART, (uint8_t)ch);
	while((USART_GetFlagStatus(APP_TRACE_USART, USART_FLAG_TC) == RESET) && (time_out != 0))
	{
		time_out--;
	}
	return (ch);
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void UartSendMultiBytes(uint8_t * pbuf,uint16_t len)
{
	uint16_t time_out = 10000;
	
	for(uint16_t i = 0;i < len;i ++)
	{
		USART_SendData8(APP_TRACE_USART, (uint8_t)*(pbuf + i));
		while((USART_GetFlagStatus(APP_TRACE_USART, USART_FLAG_TC) == RESET) && (time_out != 0))
		{
			time_out--;
		}
	}
}

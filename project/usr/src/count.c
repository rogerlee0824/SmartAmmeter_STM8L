#include "count.h"
#include "includes.h"
#include "app_trace.h"

count_event_t count_event;
static uint8_t count_a = 0,count_b = 0;
static uint32_t AmmeterCount = 0;

/***********************************************************************
  * @brief  Initialise the resource for count.
  * @param  None
  * @retval None
************************************************************************/
void count_init(void)
{
	#ifdef COUNT_DEBUG
		printf("[COUNT] count_init\r\n");
	#endif

	GPIO_Init(GPIO_PORT_COUNT_VCC, GPIO_PIN_COUNT_VCC, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_SetBits(GPIO_PORT_COUNT_VCC, GPIO_PIN_COUNT_VCC);
	
	/* Disable interrupts */
	disableInterrupts();
	
	GPIO_Init(GPIO_PORT_COUNT_A, GPIO_PIN_COUNT_A, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_COUNT_A, EXTI_Trigger_COUNT_A);
	
	GPIO_Init(GPIO_PORT_COUNT_B, GPIO_PIN_COUNT_B, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_COUNT_B, EXTI_Trigger_COUNT_B);
	
	/* Enable interrupts */
	enableInterrupts();
	EXTI_ClearITPendingBit(EXTI_IT_PIN_COUNT_A);
	EXTI_ClearITPendingBit(EXTI_IT_PIN_COUNT_B);
}

/***********************************************************************
  * @brief  Update the LCD for count test.
* @param  pucSrcBuffer: The data to be transmitted
  * @retval None
************************************************************************/
#ifdef COUNT_TEST
void UpdataLCD(uint8_t * pucSrcBuffer)
{
	uint8_t ucTemp[6] = {0},buffer_tx[12] = {0},i = 0;
	
	if(pucSrcBuffer != NULL)
	{
		buffer_tx[0] = PACKET_HEADER;
		buffer_tx[1] = PACKET_HEADER;
		buffer_tx[2] = 0x68;
		buffer_tx[3] = 6;
    
		// Prepares the data to be displayed.
		for(i = 0;i < 5;i ++)
		{
			ucTemp[i] = (i % 2) ? (pucSrcBuffer[4-(i/2)] >> 4) : pucSrcBuffer[4-(i/2)] & 0x0F;
		}
	
		// Loads the data to be displayed.
		for(i = 4;i < 10;i ++)
		{
	    	buffer_tx[i] = ucTemp[i - 4];
		}
		buffer_tx[11] = PACKET_END;	
		// Sends the data to LCD by uart in 2400bps.
		UartSendMultiBytes(buffer_tx,sizeof(buffer_tx));
	}
}
#endif

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
static void CountMeter(void)
{
	AmmeterCount++;
	if(AmmeterCount >= 999999)//·­±í
	{
		AmmeterCount = 0;
	}
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void do_count(void)
{
	uint8_t flag_count_change = 0;
	
	if((count_a == 1)&&(count_b == 1))
	{
		count_a = 0;
		count_b = 0;
		flag_count_change = 1;
	}
	
	if(flag_count_change == 1)
	{
		CountMeter();
	}
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
void count_event_handler(void * p_event_data, uint16_t event_size)
{
	count_event_t * count_event_tmp = p_event_data;

	
	switch(count_event_tmp->eCount_event)
	{
		case COUNT_INIT:
			count_init();
			break;
		
		case COUNT_A_INT:
			#ifdef COUNT_DEBUG
				printf("[COUNT] COUNT_A\r\n");
			#endif
			if(!count_a)
			{
				count_a = 1;
			}
			do_count();
			#ifdef COUNT_TEST
		UpdataLCD(NULL);
		#endif
			break;
			
		case COUNT_B_INT:
			#ifdef COUNT_DEBUG
				printf("[COUNT] COUNT_B\r\n");
			#endif
			if(!count_b)
			{
				count_b = 1;
			}
			do_count();
			#ifdef COUNT_TEST
			UpdataLCD(NULL);
			#endif
			break;
			
		default:
			break;
	}
}
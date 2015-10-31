#include "count.h"
#include "includes.h"
#include "app_trace.h"

count_event_t count_event;
static uint8_t count_a = 0,count_b = 0;
static uint8_t u8BCDCount[5] = {0x00,0x00,0x00,0x00,0x00};
volatile uint32_t AmmeterCount = 0;
uint8_t count_a_int = 0,count_b_int = 0;

uint32_t ConvertBCD_to_HEX(uint8_t * pu8Array);

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

	// Disable interrupts 
	disableInterrupts();
	
	GPIO_Init(GPIO_PORT_COUNT_VCC, GPIO_PIN_COUNT_VCC, GPIO_Mode_Out_OD_HiZ_Fast);

	GPIO_Init(GPIO_PORT_COUNT_A, GPIO_PIN_COUNT_A, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_COUNT_A, EXTI_Trigger_COUNT_A);
	
	GPIO_Init(GPIO_PORT_COUNT_B, GPIO_PIN_COUNT_B, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_COUNT_B, EXTI_Trigger_COUNT_B);

	EXTI_ClearITPendingBit(EXTI_IT_PIN_COUNT_A);
	EXTI_ClearITPendingBit(EXTI_IT_PIN_COUNT_B);
	
	// Enable interrupts 
	enableInterrupts();
}

/***********************************************************************
  * @brief  Initialise the resource for count.
  * @param  None
  * @retval None
************************************************************************/
void count_init_AorB(uint8_t temp)
{
	#ifdef COUNT_DEBUG
		printf("[COUNT] count_init_AorB\r\n");
	#endif

	// Disable interrupts 
	disableInterrupts();

	if(temp)
	{	
		GPIO_Init(GPIO_PORT_COUNT_B, GPIO_PIN_COUNT_B, GPIO_Mode_Out_PP_High_Fast);
		GPIO_Init(GPIO_PORT_COUNT_A, GPIO_PIN_COUNT_A, GPIO_Mode_In_FL_IT);
    	EXTI_SetPinSensitivity(EXTI_PIN_COUNT_A, EXTI_Trigger_COUNT_A);
	}
	else
	{
		GPIO_Init(GPIO_PORT_COUNT_A, GPIO_PIN_COUNT_A, GPIO_Mode_Out_PP_High_Fast);
		GPIO_Init(GPIO_PORT_COUNT_B, GPIO_PIN_COUNT_B, GPIO_Mode_In_FL_IT);
    	EXTI_SetPinSensitivity(EXTI_PIN_COUNT_B, EXTI_Trigger_COUNT_B);
	}

	// Enable interrupts 
	enableInterrupts();
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
  * @brief  Convert a uint32_t data to a uint8_t array
  * @param  ulCounterTmp: The data to be transmitted
  * @param  pucArrar: The data to be transmitted
  * @retval None
************************************************************************/
void ConvertHEX_to_BCD(uint32_t u32Source, uint8_t *pu8DstArrar)
{
  	uint8_t a = 0;
  	uint8_t b = 0;
  
  	a  = u32Source/10000000UL;
  	b  = a<<4;
  	a  = u32Source%10000000UL/1000000UL;
  	b |= a;
  	pu8DstArrar[1] = b;	
  	a  = u32Source%10000000UL%1000000UL/100000UL;
  	b  = a<<4;
  	a  = u32Source%10000000UL%1000000UL%100000UL/10000UL;
  	b |= a;
  	pu8DstArrar[2]  = b;	
  	a  = u32Source%10000000UL%1000000UL%100000UL%10000UL/1000UL;
  	b  = a<<4;
  	a  = u32Source%10000000UL%1000000UL%100000UL%10000UL%1000UL/100UL;
  	b |= a;
  	pu8DstArrar[3]  = b;		
  	a  = u32Source%10000000UL%1000000UL%100000UL%10000UL%1000UL%100UL/10UL;
 	b  = a<<4;
  	a  = u32Source%10000000UL%1000000UL%100000UL%10000UL%1000UL%100UL%10UL;
  	b |= a;
  	pu8DstArrar[4]  = b;  
}

/***********************************************************************
  * @brief  Convert a uint8_t array in bcd formats to a uint32_t data 
  * @param  pu8Array: 
  * @retval The uint32_t data
************************************************************************/
uint32_t ConvertBCD_to_HEX(uint8_t * pu8Array)
{
 	uint32_t a;
    uint32_t ret;

 	a = pu8Array[1];
 	a >>= 4;
 	ret  = a*10000000UL;
 	a = pu8Array[1]&0x0f;
 	ret += a*1000000UL;	
 	a = pu8Array[2];
 	a >>= 4;
 	ret += a*100000UL;
 	a = pu8Array[2]&0x0f; 
 	ret += a*10000UL;	
 	a = pu8Array[3];
 	a >>= 4;
 	ret += a*1000UL;
 	a = pu8Array[3]&0x0f;
 	ret += a*100UL;	
 	a = pu8Array[4];
 	a >>= 4;
 	ret += a*10UL;
 	a = pu8Array[4]&0x0f;
 	ret += a;
	return ret;
}

/***********************************************************************
  * @brief  Initialise the trace interface.
  * @param  None
  * @retval None
************************************************************************/
static void CountMeter(void)
{
	#ifdef COUNT_DEBUG
		printf("[COUNT] CountMeter...\r\n");
	#endif
	AmmeterCount ++;
	if(AmmeterCount >= 999999)//·­±í
	{
		AmmeterCount = 0;
	}
	ConvertHEX_to_BCD(AmmeterCount, u8BCDCount);
	#ifdef COUNT_TEST
		UpdataLCD(u8BCDCount);
	#endif
	#ifdef COUNT_DEBUG
		printf("%02x %02x %02x %02x %02x\r\n",u8BCDCount[0],u8BCDCount[1],u8BCDCount[2],u8BCDCount[3],u8BCDCount[4]);
	#endif
}

/***********************************************************************
  * @brief  Do count.
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
  * @brief  Deal with the event handler for count.
  * @param  p_event_data
  * @param  event_size
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
			
			if(count_a_int)
			{
				count_a_int = 0;
				if(!count_a)
				{
					count_a = 1;
				}
				do_count();
			}
			break;
			
		case COUNT_B_INT:
			#ifdef COUNT_DEBUG
				printf("[COUNT] COUNT_B\r\n");
			#endif

			if(count_b_int)
			{
				count_b_int = 0;
				if(!count_b)
				{
					count_b = 1;
				}
				do_count();
			}
			break;
			
		default:
			break;
	}
}
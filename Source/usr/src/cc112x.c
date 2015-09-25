#include "cc112x.h"
#include "includes.h"
#include "stm8l15x.h"

cc112x_event_t cc112x_event;

void cc112x_init(void)
{
    #ifdef CC112x_DEBUG
	    printf("[CC112X] cc112x_init...\r\n");   
	#endif

	/* Disable interrupts */
	disableInterrupts();
	
	trxRfSpiInterfaceInit();
	
	GPIO_Init(GPIO_PORT_CC112X_TE, GPIO_PIN_CC112X_TE, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_CC112X_RE, GPIO_PIN_CC112X_RE, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_CC112X_RESET, GPIO_PIN_CC112X_RESET, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_CC112X_GPIO2, GPIO_PIN_CC112X_GPIO2, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_CC112X_GPIO3, GPIO_PIN_CC112X_GPIO3, GPIO_Mode_Out_PP_Low_Fast);

	GPIO_Init(GPIO_PORT_CC112X_GPIO0, GPIO_PIN_CC112X_GPIO0, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_CC112X_GPIO0, EXTI_Trigger_CC112X_GPIO0);
	
	/* Enable interrupts */
	enableInterrupts();
	EXTI_ClearITPendingBit(EXTI_IT_PIN_CC112X_GPIO0);
}

/***********************************************************************
  * @brief  Deal with the event handler for cc112x.
  * @param  p_event_data
  * @param  event_size
  * @retval None
************************************************************************/
void cc112x_event_handler(void * p_event_data, uint16_t event_size)
{
    cc112x_event_t * p_cc112x_event_temp = p_event_data;
	uint8_t part_num = 0;
	uint8_t i,readByte;
    
    switch(p_cc112x_event_temp->eCC112x_event)
    {
        case CC112X_INIT_EVENT:
			#ifdef CC112x_DEBUG
			    printf("[CC112X] CC112X_INIT_EVENT...\r\n");   
			#endif

			cc112x_init();
			GPIO_ResetBits(GPIO_PORT_CC112X_RESET, GPIO_PIN_CC112X_RESET);
			delay1ms(1);
			GPIO_SetBits(GPIO_PORT_CC112X_RESET, GPIO_PIN_CC112X_RESET);

			/* Power Up Reset */
			rf_PowerUpReset();
			registerConfig();
			
			#ifdef CC112x_DEBUG
			    printf("[CC112X] registerConfiging OK...\r\n"); 
				cc112xSpiReadReg(CC112X_PARTNUMBER, &part_num, 1);
				printf("[CC112X] cc112xSpiReadReg,part_num = 0x%02x\r\n",part_num);   
			#endif
			
			SPI_DeInit(APP_CC112X_SPI);
			GPIO_DeInit(GPIO_PORT_CC112X_SPI_NSS);
            break;
            
        case CC112X_TRANSMIT_EVENT:
			#ifdef CC112x_DEBUG
			    printf("[CC112X] CC112X_TRANSMIT_EVENT...\r\n");   
			#endif

			trxRfSpiInterfaceInit();

			#ifdef CC112x_DEBUG	
				printf("[CC112X] cc112xSpiReadReg,part_num = 0x%02x\r\n",part_num);	 
				for(i = 0;i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) 
				{
					cc112xSpiReadReg(preferredSettings[i].addr, &readByte, 1);
					printf("[CC112X] i = %d : 0x%02x \r\n",i,readByte); 
				}
			#endif
			
			SPI_DeInit(APP_CC112X_SPI);
			GPIO_DeInit(GPIO_PORT_CC112X_SPI_NSS);
            break;
            
        case CC112X_RECEIVE_EVENT:
			trxRfSpiInterfaceInit();

			#ifdef CC112x_DEBUG	
				cc112xSpiReadReg(CC112X_PARTNUMBER, &part_num, 1);
				printf("[CC112X] cc112xSpiReadReg,part_num = 0x%02x\r\n",part_num);	 
				for(i = 0;i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) 
				{
					cc112xSpiReadReg(preferredSettings[i].addr, &readByte, 1);
					printf("[CC112X] i = %d : 0x%02x \r\n",i,readByte); 
				}
			#endif
			
			SPI_DeInit(APP_CC112X_SPI);
			GPIO_DeInit(GPIO_PORT_CC112X_SPI_NSS);
            break;
            
        default:
            break;
    }
}

void cc112x_event_handler_test(void)
{
	uint8_t part_num = 0;
	
	cc112x_init();
	GPIO_ResetBits(GPIO_PORT_CC112X_RESET, GPIO_PIN_CC112X_RESET);
	delay1ms(1);
	GPIO_SetBits(GPIO_PORT_CC112X_RESET, GPIO_PIN_CC112X_RESET);

	/* Power Up Reset */
	rf_PowerUpReset();

	registerConfig();
#ifdef CC112x_DEBUG
	    printf("[CC112X] registerConfiging OK...\r\n"); 
#endif

	cc112xSpiReadReg(CC112X_PARTNUMBER, &part_num, 1);
#ifdef CC112x_DEBUG
	   printf("[CC112X] cc112xSpiReadReg,part_num = 0x%02x\r\n",part_num);   
#endif

}
    

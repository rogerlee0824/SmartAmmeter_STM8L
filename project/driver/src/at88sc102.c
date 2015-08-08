#include "at88sc102.h"
#include "includes.h"

/***********************************************************************
  * @brief  Initialize AT88SC102 for IC card insert
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_Init(void)
{
    /* Disable interrupts */
	disableInterrupts();
	
	#ifdef IC_CARD_DEBUG
		printf("[IC] IC_Init...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_BIT, GPIO_PIN_IC_CARD_BIT, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_IC_CARD_BIT, EXTI_Trigger_IC_CARD_BIT);
	
	/* Enable interrupts */
	enableInterrupts();
	EXTI_ClearITPendingBit(EXTI_IT_PIN_IC_CARD_BIT);
}

/***********************************************************************
  * @brief  Power on AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_PowerON(void)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_PowerON...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_PW, GPIO_PIN_IC_CARD_PW, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Reset AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_Reset(void)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_Reset...\r\n");
	#endif
        
    GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_PP_Low_Fast); 
	GPIO_Init(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_I2C_Init(void)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_I2C_Init...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_I2C_Read(uint8 * pDstBuffer)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_I2C_Init...\r\n");
	#endif
        
	GPIO_Init(GPIO_PORT_IC_CARD_CLK, GPIO_PIN_IC_CARD_CLK, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Initialize the I2C interface for AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_I2C_Write(uint8 * pSrcBuffer,uint16_t size)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_I2C_Init...\r\n");
	#endif
    
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_Read(uint8 * pDstBuffer)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_Read...\r\n");
	#endif
        
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_In_FL_No_IT);
    GPIO_ResetBits(GPIO_PORT_IC_CARD_PGM, GPIO_PIN_IC_CARD_PGM);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
    
    AT88SC102_I2C_Read(pDstBuffer);
}

/***********************************************************************
  * @brief  Read date from AT88SC102
  * @param  None
  * @retval None
************************************************************************/
void AT88SC102_Write(uint8 * pSrcBuffer,uint16_t size)
{
	#ifdef IC_CARD_DEBUG
		printf("[IC] AT88SC102_Write...\r\n");
	#endif
        
    GPIO_Init(GPIO_PORT_IC_CARD_IO, GPIO_PIN_IC_CARD_IO, GPIO_Mode_In_FL_No_IT);
    GPIO_ResetBits(GPIO_PORT_IC_CARD_PGM, GPIO_PIN_IC_CARD_PGM);
	GPIO_ResetBits(GPIO_PORT_IC_CARD_RST, GPIO_PIN_IC_CARD_RST);
    
    AT88SC102_I2C_Write(pSrcBuffer,size);
}
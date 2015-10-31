/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "main.h"
#include "app_scheduler.h"
#include "app_trace.h"
#include <stdio.h>
#include "includes.h"
#include "os_error.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
static void CLK_Config(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	float fVoltage = 0;
	
    delay1s(5);
	CLK_Config();

	sim();									// disable interrupts

	GPIO_Init(GPIOA, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOB, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOC, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOD, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOE, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOF, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIOG, GPIO_Pin_All, GPIO_Mode_Out_PP_Low_Fast);

	#ifdef DEBUG
		AppTrace_Init();
    	printf("\r\nSTM8L152 Start ...\r\n");
	#endif
	
	LED_Init();
    LED1_ON();
	scheduler_init();

	// Build count init event 
	count_event.eCount_event = COUNT_INIT;
	app_sched_event_put(&count_event,sizeof(count_event),count_event_handler);
	
	// Build key init event 
	key_event.eKey_event = KEY_INIT;
	app_sched_event_put(&key_event,sizeof(key_event),key_event_handler);
	
	// Build the valve standby event 
	valve_event.eValve_event = VALVE_STANDBY_EVENT;
	app_sched_event_put(&valve_event,sizeof(valve_event),valve_event_handler);
    
    // Build the LCD init event 
	lcd_event.eLcd_event = LCD_INIT;
	app_sched_event_put(&lcd_event,sizeof(lcd_event),lcd_event_handler);

	// Build the beeper init event 
	beeper_event.eBeeper_event = BEEPER_INIT;
	app_sched_event_put(&beeper_event,sizeof(beeper_event),beeper_event_handler);

	// Build the cc1120 Init event 
	cc112x_event.eCC112x_event = CC112X_INIT_EVENT;
	app_sched_event_put(&cc112x_event,sizeof(cc112x_event),cc112x_event_handler);

	// Build the IC card Init event 
	ic_card_event.eIC_event = IC_CARD_INIT;
	app_sched_event_put(&ic_card_event,sizeof(ic_card_event),ic_event_handler);

	//battery_measure(&fVoltage);
	//printf("[Battery] End voltage is %f\r\n",fVoltage);
	
    // enable interrupts 
	rim();
    LED1_OFF();
	 
	// Infinite loop 
  	while (1)
  	{
		app_sched_execute();
		app_evt_wait();
  	}
}

/***********************************************************************
  * @brief  IdleTask
  * @param  None
  * @retval None
************************************************************************/
void IdleTask(void)
{	
	#ifdef DEBUG
		printf("IdleTask ...\r\n");
		AppTrace_DeInit();
	#endif

	PWR_UltraLowPowerCmd(ENABLE);
	CLK_HaltConfig(CLK_Halt_FastWakeup, ENABLE);
	PWR_FastWakeUpCmd(ENABLE);
    halt();

	#ifdef DEBUG
		AppTrace_Init();
		printf("Wakeup ...\r\n");
	#endif
}

/***********************************************************************
  * @brief		Configure System Clock 
  * @param	None
  * @retval   None
************************************************************************/
static void CLK_Config(void)
{
  	// Select HSE as system clock source 
  	CLK_SYSCLKSourceSwitchCmd(ENABLE);
  	CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
  
  	// system clock prescaler: 1
  	CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  	while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)
  	{}
}

#ifdef  USR_ASSERT
/******************************************************************************
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
*******************************************************************************/
void usr_assert_failed(uint8_t * file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	//printf("Wrong parameters,file: %s, line: %d\r\n", file, line);
}
#endif

#ifdef  USR_ERROR_CHECK
/******************************************************************************
 * @brief Function for error handling, which is called when an error has occurred.
 * @warning This handler is an example only and does not fit a final product. 
 *	You need to analyze how your product is supposed to react in case of error.
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name.
*******************************************************************************/
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t * p_file_name)
{
	//printf("Error NO.: %d, File: %s, Line: %d\r\n", error_code, p_file_name, line_num);
}
	
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

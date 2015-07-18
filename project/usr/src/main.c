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

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
	disableInterrupts();
	app_trace_init();
	printf("Hello!!!\r\n");
	LED_Init();
	scheduler_init();

	count_event.eCount_event = COUNT_INIT;
	app_sched_event_put(&count_event,sizeof(count_event),count_event_handler);
	
	//key_event.eKey_event = KEY_INIT;
	//app_sched_event_put(&key_event,sizeof(key_event),key_event_handler);
	
	/* enable interrupts */
	enableInterrupts();
	
	/* Infinite loop */
  	while (1)
  	{
		app_sched_execute();
		app_evt_wait();
  	}
}

#ifdef  USR_ASSERT
/******************************************************************************
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
*******************************************************************************/
void usr_assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	printf("Wrong parameters,file: %s, line: %d\r\n", file, line);
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
	printf("Error NO.: %d, File: %s, Line: %d\r\n", error_code, p_file_name, line_num);
}
	
#endif

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

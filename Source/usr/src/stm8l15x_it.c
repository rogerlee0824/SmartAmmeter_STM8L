/**
  ******************************************************************************
  * @file    Project/STM8L15x_StdPeriph_Template/stm8l15x_it.c
  * @author  MCD Application Team
  * @version V1.6.1
  * @date    30-September-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all peripherals interrupt service routine.
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
#include "stm8l15x_it.h"
#include <stdio.h>
#include "includes.h"
#include "stm8l15x_exti.h"

/** @addtogroup STM8L15x_StdPeriph_Template
  * @{
  */
	
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

#ifdef _COSMIC_
/**
  * @brief Dummy interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER(NonHandledInterrupt,0)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
#endif

/**
  * @brief TRAP interrupt routine
  * @par Parameters:
  * None
  * @retval 
  * None
*/
INTERRUPT_HANDLER_TRAP(TRAP_IRQHandler)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief FLASH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(FLASH_IRQHandler,1)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel0 and channel1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL0_1_IRQHandler,2)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief DMA1 channel2 and channel3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(DMA1_CHANNEL2_3_IRQHandler,3)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief RTC / CSS_LSE Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(RTC_CSSLSE_IRQHandler,4)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
    if(RTC_GetFlagStatus(RTC_FLAG_WUTWF))
    {
    	RTC_ClearFlag(RTC_FLAG_WUTWF);
    	RTC_ClearITPendingBit(RTC_IT_WUT);
    }
}
/**
  * @brief External IT PORTE/F and PVD Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIE_F_PVD_IRQHandler,5)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
  if (PWR_PVDGetITStatus() != RESET)
  {
    PWR_PVDClearITPendingBit();
    if (PWR_GetFlagStatus(PWR_FLAG_PVDOF) != RESET)
    {
    	/* Falling Direction Detected*/
		PWR_PVDClearFlag();
		pvd_event.emPVD_event = PVD_FALLING_EVENT;
		app_sched_event_put(&pvd_event,sizeof(pvd_event),PVD_event_handler);
    }
    else
    {
    	/* Rising Direction Detected*/
		PWR_PVDClearFlag();
		pvd_event.emPVD_event = PVD_RISING_EVENT;
		app_sched_event_put(&pvd_event,sizeof(pvd_event),PVD_event_handler);
    }
  }
}

/**
  * @brief External IT PORTB / PORTG Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTIB_G_IRQHandler,6)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PORTD /PORTH Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTID_H_IRQHandler,7)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN0 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI0_IRQHandler,8)
{
    if(EXTI_GetITStatus(EXTI_IT_Pin0))
    {
        /* Cleat Interrupt pending bit */
  		EXTI_ClearITPendingBit(EXTI_IT_Pin0);
        
        /* Check if the interrupt is from the COUNT_A pin or not */
        if(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_GPIO0, GPIO_PIN_CC112X_GPIO0) == RESET)
        {
            cc112x_event.eCC112x_event = CC112X_RECEIVE_EVENT;
			app_sched_event_put(&cc112x_event,sizeof(cc112x_event_t),cc112x_event_handler);
        }
    }
}

/**
  * @brief External IT PIN1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI1_IRQHandler,9)
{
    if(EXTI_GetITStatus(EXTI_IT_PIN_KEY))
    {
		/* Cleat Interrupt pending bit */
  		EXTI_ClearITPendingBit(EXTI_IT_Pin1);
        
        /* Check if the key is pressed or not */
        if(GPIO_ReadInputDataBit(GPIO_PORT_KEY, GPIO_PIN_KEY) == RESET)
        {
        	if(!key_is_pressed)
        	{
        		key_is_pressed = 1;
            	#ifdef KEY_DEBUG
					AppTrace_Init();
                	printf("EXTI1_IRQHandler!!!\r\n");
            	#endif
            	key_event.eKey_event = KEY_HANDLE;
            	app_sched_event_put(&key_event,sizeof(key_event),key_event_handler);
        	}
        }

		 /* Check if the GPIO2 is interrupt or not */
        if(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_GPIO2, GPIO_PIN_CC112X_GPIO2) == RESET)
        {
            #ifdef KEY_DEBUG
				AppTrace_Init();
                printf("GPIO2_IRQHandler!!!\r\n");
            #endif
        }
    }
}

/**
  * @brief External IT PIN2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI2_IRQHandler,10)
{
    if(EXTI_GetITStatus(EXTI_IT_Pin2))
    {
        /* Cleat Interrupt pending bit */
  		EXTI_ClearITPendingBit(EXTI_IT_Pin2);
		#ifdef COUNT_DEBUG
			AppTrace_Init();
			printf("[COUNT] EXTI2_IRQHandler,%d\r\n",count_a_int);
		#endif

		if(!count_a_int)
		{
			count_init_AorB(0);
			count_a_int = 1;
				
            count_event.eCount_event = COUNT_A_INT;
            app_sched_event_put(&count_event,sizeof(count_event),count_event_handler);
        }
    }
}

/**
  * @brief External IT PIN3 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI3_IRQHandler,11)
{
	if(EXTI_GetITStatus(EXTI_IT_Pin3))
    {
    	/* Cleat Interrupt pending bit */
  		EXTI_ClearITPendingBit(EXTI_IT_Pin3);
		#ifdef COUNT_DEBUG
			AppTrace_Init();
			printf("[COUNT] EXTI3_IRQHandler,%d\r\n",count_b_int);
		#endif

		if(!count_b_int)
		{
			count_init_AorB(1);
			count_b_int = 1;

			count_event.eCount_event = COUNT_B_INT;
			app_sched_event_put(&count_event,sizeof(count_event),count_event_handler);
		}
	}
}

/**
  * @brief External IT PIN4 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI4_IRQHandler,12)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

}

/**
  * @brief External IT PIN5 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI5_IRQHandler,13)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN6 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI6_IRQHandler,14)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief External IT PIN7 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(EXTI7_IRQHandler,15)
{
    if(EXTI_GetITStatus(EXTI_IT_PIN_IC_CARD_BIT))
    {
        /* Cleat Interrupt pending bit */
  		EXTI_ClearITPendingBit(EXTI_IT_PIN_IC_CARD_BIT);
        
        /* Check if the IC card is inserted or not */
        if(GPIO_ReadInputDataBit(GPIO_PORT_IC_CARD_BIT, GPIO_PIN_IC_CARD_BIT) == RESET)
        {
            #ifdef IC_CARD_DEBUG
				AppTrace_Init();
                printf("IC Card insert!!!\r\n");
            #endif
            ic_card_event.eIC_event = IC_CARD_INSERT;
            app_sched_event_put(&ic_card_event,sizeof(ic_event_t),ic_event_handler);
        }
    }
}
/**
  * @brief LCD /AES Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(LCD_AES_IRQHandler,16)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief CLK switch/CSS/TIM1 break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SWITCH_CSS_BREAK_DAC_IRQHandler,17)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief ADC1/Comparator Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(ADC1_COMP_IRQHandler,18)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief TIM2 Update/Overflow/Trigger/Break /USART2 TX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_USART2_TX_IRQHandler,19)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief Timer2 Capture/Compare / USART2 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM2_CC_USART2_RX_IRQHandler,20)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}


/**
  * @brief Timer3 Update/Overflow/Trigger/Break Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_USART3_TX_IRQHandler,21)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief Timer3 Capture/Compare /USART3 RX Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM3_CC_USART3_RX_IRQHandler,22)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief TIM1 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_CC_IRQHandler,24)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

  /* Clear TIM1 Capture compare interrupt pending bit */
  TIM1_ClearITPendingBit(TIM1_IT_CC3);

  /* Get the Input Capture value by reading CCR1 register */
  /* CCR1 regsiter contains signal frequency value */
  IC1Value = TIM1_GetCapture3();

  	timer1_event.emTimer1_event = TIMER1_CAPTURE_END;
	app_sched_event_put(&timer1_event,sizeof(timer1_event),timer1_event_handler);
}

/**
  * @brief TIM4 Update/Overflow/Trigger Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM4_UPD_OVF_TRG_IRQHandler,25)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @brief SPI1 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(SPI1_IRQHandler,26)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */		
}

/**
  * @brief USART1 TX / TIM5 Update/Overflow/Trigger/Break Interrupt  routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_TX_TIM5_UPD_OVF_TRG_BRK_IRQHandler,27)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief USART1 RX / Timer5 Capture/Compare Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(USART1_RX_TIM5_CC_IRQHandler,28)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}

/**
  * @brief I2C1 / SPI2 Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(I2C1_SPI2_IRQHandler,29)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */
}
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
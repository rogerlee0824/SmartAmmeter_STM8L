#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <stdio.h>
#include "stm8l15x.h"
#include "main.h"
#include "app_trace.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "count.h"
#include "valve.h"
#include "Beeper.h"
#include "lcd_sc.h"
#include "ic.h"
#include "cc112x.h"
#include "cc112x_spi.h"
#include "hal_spi_rf_trxeb.h"
#include "cc1120_sniff_mode.h"
#include "app_scheduler.h"
#include "battery.h"
#include "flash.h"
#include "dataConvert.h"
#include "pvd.h"
#include "timer1.h"


/* Define debug function -------------------------------------------------------*/
#define DEBUG

#ifdef DEBUG
#define KEY_DEBUG
#define LCD_DEBUG
#define COUNT_DEBUG
//#define COUNT_TEST
//#define VALVE_DEBUG
//#define IC_CARD_DEBUG
//#define SLE4442_DEBUG
#define CC112x_DEBUG
//#define BEEPER_DEBUG
//#define BATTERY_DEBUG
//#define DATAMEM_DEBUG
#define PVD_DEBUG
#define TIMER1_DEBUG
#endif


#define STM8L_SUCCESS                           (0)  // Successful command
#define STM8L_FAIL                           	(1)  // Fail command

#define GAS_COEFFICIENT							(10)

/* Define Trace pins -----------------------------------------------------------*/
#define APP_TRACE_USART							USART3
#define GPIO_PORT_USART_TX  					GPIOG
#define GPIO_PIN_USART_TX  						GPIO_Pin_1
#define GPIO_PORT_USART_RX  					GPIOG
#define GPIO_PIN_USART_RX  						GPIO_Pin_0


/* Define LED pins ------------------------------------------------------------*/
#define GPIO_PORT_LED1  						GPIOC
#define GPIO_PIN_LED1  							GPIO_Pin_4

/* Define KEY pins -----------------------------------------------------------*/
#define GPIO_PORT_KEY  							GPIOC
#define GPIO_PIN_KEY  							GPIO_Pin_1
#define EXTI_PIN_KEY 							EXTI_Pin_1
#define EXTI_Trigger_KEY 						EXTI_Trigger_Falling
#define EXTI_IT_PIN_KEY							EXTI_IT_Pin1

/* Define COUNT pins -----------------------------------------------------------*/
#define GPIO_PORT_COUNT_VCC  					GPIOG
#define GPIO_PIN_COUNT_VCC  					GPIO_Pin_0

#define GPIO_PORT_COUNT_A  						GPIOA
#define GPIO_PIN_COUNT_A  						GPIO_Pin_2
#define EXTI_PIN_COUNT_A 						EXTI_Pin_2
#define EXTI_Trigger_COUNT_A 					EXTI_Trigger_Falling
#define EXTI_IT_PIN_COUNT_A						EXTI_IT_Pin2

#define GPIO_PORT_COUNT_B  						GPIOA
#define GPIO_PIN_COUNT_B  						GPIO_Pin_3
#define EXTI_PIN_COUNT_B 						EXTI_Pin_3
#define EXTI_Trigger_COUNT_B 					EXTI_Trigger_Falling
#define EXTI_IT_PIN_COUNT_B						EXTI_IT_Pin3

/* Define MOTOR pins ------------------------------------------------------------*/
#define GPIO_PORT_VALVE_IA  					GPIOG
#define GPIO_PIN_VALVE_IA  						GPIO_Pin_3
#define GPIO_PORT_VALVE_IB  					GPIOG
#define GPIO_PIN_VALVE_IB  						GPIO_Pin_2
#define GPIO_PORT_VALVE_MONITOR  				GPIOF
#define GPIO_PIN_VALVE_MONITOR  				GPIO_Pin_1

/* Define CC112x  pins ------------------------------------------------------------*/
#define APP_CC112X_SPI							SPI1

#define GPIO_PORT_CC112X_SPI_NSS  				GPIOB
#define GPIO_PIN_CC112X_SPI_NSS  				GPIO_Pin_4
#define GPIO_PORT_CC112X_SPI_SCK 				GPIOB
#define GPIO_PIN_CC112X_SPI_SCK  				GPIO_Pin_5
#define GPIO_PORT_CC112X_SPI_MOSI  				GPIOB
#define GPIO_PIN_CC112X_SPI_MOSI  				GPIO_Pin_6
#define GPIO_PORT_CC112X_SPI_MISO  				GPIOB
#define GPIO_PIN_CC112X_SPI_MISO  				GPIO_Pin_7
#define GPIO_PORT_CC112X_TE  					GPIOF
#define GPIO_PIN_CC112X_TE  					GPIO_Pin_6
#define GPIO_PORT_CC112X_RE  					GPIOF
#define GPIO_PIN_CC112X_RE  					GPIO_Pin_7
#define GPIO_PORT_CC112X_RESET  				GPIOF
#define GPIO_PIN_CC112X_RESET  					GPIO_Pin_5
#define GPIO_PORT_CC112X_GPIO2  				GPIOD
#define GPIO_PIN_CC112X_GPIO2   				GPIO_Pin_5
#define GPIO_PORT_CC112X_GPIO3   				GPIOF
#define GPIO_PIN_CC112X_GPIO3   				GPIO_Pin_4
#define GPIO_PORT_CC112X_GPIO0  				GPIOF
#define GPIO_PIN_CC112X_GPIO0  					GPIO_Pin_0

#define EXTI_PIN_CC112X_GPIO0 					EXTI_Pin_0
#define EXTI_Trigger_CC112X_GPIO0 				EXTI_Trigger_Falling
#define EXTI_IT_PIN_CC112X_GPIO0				EXTI_IT_Pin0

#define EXTI_PIN_CC112X_GPIO2 					EXTI_Pin_1
#define EXTI_Trigger_CC112X_GPIO2 				EXTI_Trigger_Falling
#define EXTI_IT_PIN_CC112X_GPIO2				EXTI_IT_Pin1

#define EXTI_PIN_CC112X_GPIO3 					EXTI_Pin_4
#define EXTI_Trigger_CC112X_GPIO3 				EXTI_Trigger_Falling
#define EXTI_IT_PIN_CC112X_GPIO3				EXTI_IT_Pin4




/* Define LCD pins ------------------------------------------------------------*/
#define GPIO_PORT_COM0  					    GPIOA
#define GPIO_PIN_COM0  						    GPIO_Pin_4

#define GPIO_PORT_COM1  					    GPIOA
#define GPIO_PIN_COM1  						    GPIO_Pin_5

#define GPIO_PORT_COM2  					    GPIOA
#define GPIO_PIN_COM2  						    GPIO_Pin_6

#define GPIO_PORT_COM3  					    GPIOD
#define GPIO_PIN_COM3  						    GPIO_Pin_1

#define GPIO_PORT_S0  					        GPIOA
#define GPIO_PIN_S0  						    GPIO_Pin_7

#define GPIO_PORT_S1  					        GPIOE
#define GPIO_PIN_S1  						    GPIO_Pin_0

#define GPIO_PORT_S2  					        GPIOE
#define GPIO_PIN_S2  						    GPIO_Pin_1

#define GPIO_PORT_S3  					        GPIOE
#define GPIO_PIN_S3  						    GPIO_Pin_2

#define GPIO_PORT_S4  					        GPIOE
#define GPIO_PIN_S4  						    GPIO_Pin_3

#define GPIO_PORT_S5  					        GPIOE
#define GPIO_PIN_S5  						    GPIO_Pin_4

#define GPIO_PORT_S6  					        GPIOE
#define GPIO_PIN_S6  						    GPIO_Pin_5

#define GPIO_PORT_S7  					        GPIOD
#define GPIO_PIN_S7  						    GPIO_Pin_0

#define GPIO_PORT_S8  					        GPIOD
#define GPIO_PIN_S8  						    GPIO_Pin_2

#define GPIO_PORT_S9  					        GPIOD
#define GPIO_PIN_S9  						    GPIO_Pin_3

#define GPIO_PORT_S10  					        GPIOB
#define GPIO_PIN_S10  						    GPIO_Pin_0

#define GPIO_PORT_S11  					        GPIOB
#define GPIO_PIN_S11  						    GPIO_Pin_1

#define GPIO_PORT_S12  					        GPIOB
#define GPIO_PIN_S12  						    GPIO_Pin_2

#define GPIO_PORT_S13  					        GPIOB
#define GPIO_PIN_S13  						    GPIO_Pin_3

/* Define IC card pins -------------------------------------------------------*/
#define GPIO_PORT_IC_CARD_PGM  					GPIOC
#define GPIO_PIN_IC_CARD_PGM  				    GPIO_Pin_2

#define GPIO_PORT_IC_CARD_PW  					GPIOE
#define GPIO_PIN_IC_CARD_PW  				    GPIO_Pin_7

#define GPIO_PORT_IC_CARD_PRE  					GPIOG
#define GPIO_PIN_IC_CARD_PRE  				    GPIO_Pin_7

#define GPIO_PORT_IC_CARD_RST  					GPIOG
#define GPIO_PIN_IC_CARD_RST  				    GPIO_Pin_6

#define GPIO_PORT_IC_CARD_CLK  				    GPIOG
#define GPIO_PIN_IC_CARD_CLK  				    GPIO_Pin_5

#define GPIO_PORT_IC_CARD_IO  					GPIOG
#define GPIO_PIN_IC_CARD_IO  				    GPIO_Pin_4

#define GPIO_PORT_IC_CARD_BIT  				    GPIOD
#define GPIO_PIN_IC_CARD_BIT  				    GPIO_Pin_7

#define EXTI_PIN_IC_CARD_BIT  					EXTI_Pin_7
#define EXTI_Trigger_IC_CARD_BIT 				EXTI_Trigger_Falling
#define EXTI_IT_PIN_IC_CARD_BIT					EXTI_IT_Pin7

/* Define BEEP pins -----------------------------------------------------------*/
#define GPIO_PORT_BEEPER  						GPIOE
#define GPIO_PIN_BEEPER  						GPIO_Pin_6

/* Define BATTERY pins -----------------------------------------------------------*/
#define GPIO_PORT_VTEST_5V  					GPIOC
#define GPIO_PIN_VTEST_5V  						GPIO_Pin_3
#define GPIO_PORT_ADC_OUT  						GPIOD
#define GPIO_PIN_ADC_OUT  						GPIO_Pin_4
#define GPIO_PORT_AA_OUT_IN  					GPIOD
#define GPIO_PIN_AA_OUT_IN  					GPIO_Pin_6
#define GPIO_PORT_AA_CTRL  						GPIOC
#define GPIO_PIN_AA_CTRL  						GPIO_Pin_7


#define REMAIN_GAS_MIN							(GAS_COEFFICIENT * 5)	

typedef union {
	uint32_t l;
	uint8_t arr[4];
}uint32_t_or_u8_u;

/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
#define USR_ASSERT    			(1) 

/* Exported macro ------------------------------------------------------------*/
#ifdef  USR_ASSERT
/**********************************************************************************
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval : None
**********************************************************************************/
  #define usr_assert(expr) ((expr) ? (void)0 : usr_assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void usr_assert_failed(uint8_t* file, uint16_t line);
#else
  #define usr_assert(expr) ((void)0)
#endif /* USR_ASSERT */
	
#endif // End of #ifndef _INCLUDES_H_

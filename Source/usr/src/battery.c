#include "battery.h"

#define BUFFER_SIZE					(6)

uint16_t adc_value[BUFFER_SIZE] = {0};
uint16_t result = 0;


void ADC_GPIO_Init(void)
{
	#ifdef BATTERY_DEBUG
		printf("[Battery] ADC_GPIO_Init\r\n");
	#endif
	
	GPIO_Init(GPIO_PORT_VTEST_5V, GPIO_PIN_VTEST_5V, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_ADC_OUT, GPIO_PIN_ADC_OUT, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_AA_OUT_IN, GPIO_PIN_AA_OUT_IN, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_AA_CTRL, GPIO_PIN_AA_CTRL, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief  Configure ADC peripheral
  * @param  None
  * @retval None
  */
static void ADC_Config(void)
{
    /* Initialise and configure ADC1 */
    ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 Channel 3 */
    ADC_ChannelCmd(ADC1, ADC_Channel_5, ENABLE);

    /* Enable End of conversion ADC1 Interrupt */
    //ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
}

void battery_measure(float * p_bat_voltage)
{
	float voltage = 0;
	
	#ifdef BATTERY_DEBUG
		printf("[Battery] Start measuring battery ...\r\n");
	#endif

	result = 0;
	GPIO_Init(GPIO_PORT_ADC_OUT, GPIO_PIN_ADC_OUT, GPIO_Mode_Out_PP_High_Fast);
	GPIO_Init(GPIO_PORT_VTEST_5V, GPIO_PIN_VTEST_5V, GPIO_Mode_In_FL_No_IT);

	/* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

    ADC_Config();

    /* Infinite loop*/
    for(uint8_t i = 0;i < BUFFER_SIZE;i ++)
    {
        /* Start ADC1 Conversion using Software trigger*/
        ADC_SoftwareStartConv(ADC1);

        /* Wait until ADC Channel 15 end of conversion */
        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
        {}
        
        /* read ADC convertion result */
        adc_value[i] = ADC_GetConversionValue(ADC1);
		result += adc_value[i];
    }

	voltage = (result * 3.6)/(BUFFER_SIZE * 4096);
	/* Enable ADC1 */
    ADC_Cmd(ADC1, DISABLE);

	/* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
	
	GPIO_Init(GPIO_PORT_ADC_OUT, GPIO_PIN_ADC_OUT, GPIO_Mode_Out_PP_Low_Fast);
	GPIO_Init(GPIO_PORT_VTEST_5V, GPIO_PIN_VTEST_5V, GPIO_Mode_Out_PP_Low_Fast);

	*p_bat_voltage = voltage;
	
	#ifdef BATTERY_DEBUG
		for (uint8_t i = 0; i < BUFFER_SIZE; i++)
        {
            printf("[Battery] voltage is 0x%04x\r\n",adc_value[i]);
		}
	#endif
}

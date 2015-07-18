#include "key.h"
#include "includes.h"

void KEY_Init(void)
{
	GPIO_Init(GPIO_PORT_KEY, GPIO_PIN_KEY, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_PIN_KEY, EXTI_Trigger_KEY);
}
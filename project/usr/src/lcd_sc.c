#include "lcd_sc.h"
#include "includes.h"

lcd_event_t lcd_event;

void LCD_SC_Init(void)
{
    GPIO_Init(GPIO_PORT_COM0, GPIO_PIN_COM0, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_COM1, GPIO_PIN_COM1, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_COM2, GPIO_PIN_COM2, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_COM3, GPIO_PIN_COM3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S0, GPIO_PIN_S0, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S1, GPIO_PIN_S1, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S2, GPIO_PIN_S2, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S3, GPIO_PIN_S3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S4, GPIO_PIN_S4, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S5, GPIO_PIN_S5, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S6, GPIO_PIN_S6, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S7, GPIO_PIN_S7, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S8, GPIO_PIN_S8, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S9, GPIO_PIN_S9, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S10, GPIO_PIN_S10, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S11, GPIO_PIN_S11, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIO_PORT_S12, GPIO_PIN_S12, GPIO_Mode_Out_PP_Low_Fast);    
    GPIO_Init(GPIO_PORT_S13, GPIO_PIN_S13, GPIO_Mode_Out_PP_Low_Fast);
}

/***********************************************************************
  * @brief  Display the close valve icon.
  * @param  None
  * @retval None
************************************************************************/
void LCD_SC_DisplayValveClose(void)
{
    GPIO_ResetBits(GPIO_PORT_COM0, GPIO_PIN_COM0);
    GPIO_ResetBits(GPIO_PORT_COM1, GPIO_PIN_COM1);
    GPIO_ResetBits(GPIO_PORT_COM2, GPIO_PIN_COM2);
    GPIO_SetBits(GPIO_PORT_COM3, GPIO_PIN_COM3);
    GPIO_ResetBits(GPIO_PORT_S13, GPIO_PIN_S13);
}

void lcd_event_handler(void * p_event_data, uint16_t event_size)
{
	lcd_event_t * lcd_event_tmp = p_event_data;
	
	switch(lcd_event_tmp->eLcd_event)
	{
        case LCD_INIT:
            LCD_SC_Init();
            break;
            
        case lCD_HANDLE:
            LCD_SC_DisplayValveClose();
            break;
        
		default:
			break;
	}
}
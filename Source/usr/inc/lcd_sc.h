#ifndef _LCD_H_
#define _LCD_H_

#include "includes.h"

#define DISP_VALVE_CLOSE_ICON			0x0001			/* Display the icon that the valve has been closed */
#define DISP_ARREARAGE_ICON				0x0002			/* Display the icon that the user has arrearaged */
#define DISP_GAS_ICON					0x0004			/* Display the icon of the remain gas  */
#define DISP_AMOUNT_ICON				0x0008			/* Display the amount icon  */
#define DISP_RETRY_ICON					0x0010			/* Display the retry icon */
#define DISP_LOW_VOLTAGE_ICON			0x0020			/* Display the icon that indicate that the voltage is low */
#define DISP_FAULT_ICON					0x0040			/* Display the icon that the ammeter is fault */
#define DISP_CNY_ICON					0x0080			/* Display the icon that THE UNIT of the money is CNY */
#define DISP_STERE_ICON					0x0100			/* Display the icon that THE UNIT of the gas is cubic meter  */

typedef enum {
	LCD_INIT,
	LCD_DISPLAY_REMAIN_GAS,
	lCD_HANDLE,
	LCD_DEINIT
} emLcd_event_t;

typedef struct {
	emLcd_event_t eLcd_event;
	uint8_t * pData;
} lcd_event_t;

typedef struct {
	uint16_t  com0;
	uint16_t  com1;
    uint16_t  com2;
    uint16_t  com3;
} lcd_ram_t;

typedef struct {
	uint8_t  				is_disp_digits;				/* There is some digits need to be displayed */
	uint8_t	 				is_disp_icon;				/* There is some icon need to be displayed */
	uint8_t 				is_disp_dot;				/* There is a dot  need to be displayed */
	uint8_t 	   			* p_disp_contex;
	uint8_t 				disp_contex_len;
	uint16_t 				* p_disp_icon;
	uint8_t 				disp_dot_num;
} lcd_disp_info_t;

extern lcd_event_t lcd_event;
extern lcd_disp_info_t lcd_disp_info;

extern uint8_t disp_temp[6];
extern uint16_t disp_icon;

void LCD_SC_Init(void);
void LCD_SC_DisplayValveClose(void);
void lcd_event_handler(void * p_event_data, uint16_t event_size);

#endif
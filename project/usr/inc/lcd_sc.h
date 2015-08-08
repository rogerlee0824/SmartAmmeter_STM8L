#ifndef _LCD_H_
#define _LCD_H_

#include "includes.h"

typedef enum {
	LCD_INIT,
	lCD_HANDLE
} emLcd_event_t;

typedef struct {
	emLcd_event_t eLcd_event;
	uint8_t * pData;
} lcd_event_t;

extern lcd_event_t lcd_event;

void LCD_SC_Init(void);
void LCD_SC_DisplayValveClose(void);
void lcd_event_handler(void * p_event_data, uint16_t event_size);

#endif
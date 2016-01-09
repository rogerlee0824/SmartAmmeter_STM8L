#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "stm8l15x.h"
#include "includes.h"

#define BAT_INT_SHIFT_PULLUP_RES				(1000)			// (Kohm)
#define BAT_INT_SHIFT_PULLDOWN_RES				(500)			// (Kohm)

#define BAT_EXT_SHIFT_PULLUP_RES				(1500)			// (Kohm)
#define BAT_EXT_SHIFT_PULLDOWN_RES				(500)			// (Kohm)

typedef enum {  
	EXT_BATTERY_EVENT,
	INT_BATTERY_EVENT
} emBattery_event_t;

typedef struct{
	emBattery_event_t eBattery_event;
}battery_event_t;

extern battery_event_t battery_event;
extern uint16_t adc_value[];

void Battery_Int_Measure(float * p_bat_voltage);
void battery_event_handler(void * p_event_data, uint16_t event_size);

#endif
#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "stm8l15x.h"
#include "includes.h"

typedef enum {  
	ADC_INIT,
	ADC_SAMPLE,
} emBattery_event_t;

typedef struct {
    emBattery_event_t      	eBattery_event;
    uint32_t                u16FwUpdateDataLen;
    uint8_t 				* u8FwUpdateDataSource;
} battery_event_t;

extern battery_event_t battery_event;

void battery_event_handler(void * p_event_data, uint16_t event_size);

#endif
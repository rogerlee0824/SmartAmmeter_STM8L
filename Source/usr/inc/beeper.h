#ifndef _BEEPER_H_
#define _BEEPER_H_

#include "stm8l15x.h"
#include "includes.h"

#define BEEPER_TIMES_INT_BATTERY_VOLTAGE_LOW			(2)
#define BEEPER_TIMES_EXT_BATTERY_VOLTAGE_LOW			(3)
#define BEEPER_TIMES_REMEAN_GAS_LOW						(4)



typedef enum {  
	BEEPER_INIT,
	BEEPER
} emBeeper_event_t;

typedef struct{
	emBeeper_event_t eBeeper_event;
	uint8_t beeper_times;
}beeper_event_t;

extern beeper_event_t beeper_event;

extern uint8_t beeper_is_running;

void Beeper_Init(void);
void beeper_event_handler(void * p_event_data, uint16_t event_size);


#endif		// End of #ifndef _BEEPER_H_


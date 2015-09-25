#ifndef _VALVE_H_
#define _VALVE_H_

#include "stm8l15x.h"

typedef enum {VALVE_OPEN = 0, VALVE_CLOSE} emValve_state_t;

typedef enum {VALVE_OPEN_EVENT, VALVE_CLOSE_EVENT,VALVE_STANDBY_EVENT} emValve_event_t;

typedef struct {
	emValve_event_t eValve_event;
	uint8_t * pData;
} valve_event_t;

extern valve_event_t valve_event;

void valve_event_handler(void * p_event_data, uint16_t event_size);

#endif  /* #ifndef _VALVE_H_ */
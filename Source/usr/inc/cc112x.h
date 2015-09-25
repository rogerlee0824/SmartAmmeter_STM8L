#ifndef _CC112X_H_
#define _CC112X_H_

#include "stm8l15x.h"
#include "includes.h"

typedef enum {	
	CC112X_INIT_EVENT, 
	CC112X_TRANSMIT_EVENT,
	CC112X_RECEIVE_EVENT
} emCC112x_event_t;

typedef struct {
	emCC112x_event_t eCC112x_event;
	uint8_t * pData;
} cc112x_event_t;

extern cc112x_event_t cc112x_event;

void cc112x_init(void);
void cc112x_event_handler(void * p_event_data, uint16_t event_size);
void cc112x_event_handler_test(void);



#endif      /* #ifndef _CC112X_H_ */

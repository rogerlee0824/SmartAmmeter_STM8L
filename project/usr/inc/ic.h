#ifndef _IC_H_
#define _IC_H_

#include "stm8l15x.h"

typedef enum {
	IC_CARD_INIT,
	IC_CARD_INSERT
} emIC_event_t;

typedef struct {
	emIC_event_t    eIC_event;
	uint8_t         * pData;
} ic_event_t;

extern ic_event_t ic_event;

void ic_event_handler(void * p_event_data, uint16_t event_size);

#endif
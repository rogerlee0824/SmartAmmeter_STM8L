#ifndef _KEY_H_
#define _KEY_H_

#include "stm8l15x.h"

typedef enum {
	KEY_INIT,
	KEY_HANDLE
} emKey_event_t;

typedef struct {
	emKey_event_t eKey_event;
	uint8_t * pData;
} key_event_t;

extern key_event_t key_event;

void KEY_Init(void);
void key_event_handler(void * p_event_data, uint16_t event_size);

#endif

#ifndef _IC_H_
#define _IC_H_

#include "stm8l15x.h"

#define IC_CARD_GAS_START_ADDR    		(0x20)
#define IC_CARD_GAS_OFFSET    			(4)


typedef enum {
	IC_CARD_INIT,
	IC_CARD_INSERT,
	IC_CARD_DEINIT
} emIC_event_t;

typedef struct {
	emIC_event_t    eIC_event;
	uint8_t         * pData;
} ic_event_t;

typedef struct{
	uint8_t atr[4];
}atr_header_t;

typedef struct{
	uint8_t arr[4];
}manufacture_header_t;

typedef struct{
	uint8_t arr[24];
}custom_header_t;

typedef struct{
	atr_header_t atr_header;
	manufacture_header_t manufacture_header;
	custom_header_t custom_header;
}protect_area_header_t;

extern ic_event_t ic_card_event;

void ic_event_handler(void * p_event_data, uint16_t event_size);

#endif

#ifndef _COUNT_H_
#define _COUNT_H_

#include "stm8l15x.h"
#include "includes.h"

#define PACKET_HEADER               0xAA
#define PACKET_END           	    0xBB

typedef enum {  
	COUNT_INIT,
	COUNT_A_INT,
	COUNT_B_INT
} emCount_event_t;

typedef struct {
    emCount_event_t       	eCount_event;
    uint32_t                u16FwUpdateDataLen;
    uint8_t *               u8FwUpdateDataSource;
} count_event_t;

extern count_event_t count_event;

void count_init(void);
void do_count(void);
void ConvertHEX_to_BCD(uint32_t u32Source, uint8_t * pu8DstArrar);
void count_event_handler(void * p_event_data, uint16_t event_size);

#ifdef COUNT_TEST
void UpdataLCD(uint8_t * pucSrcBuffer);
#endif

#endif
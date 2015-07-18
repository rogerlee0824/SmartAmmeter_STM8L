#ifndef _APP_TRACE_H_
#define _APP_TRACE_H_

#include "stm8l15x.h"

void app_trace_init(void);
void UartSendMultiBytes(uint8_t * pbuf,uint16_t len);

#endif

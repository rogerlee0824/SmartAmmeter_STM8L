#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm8l15x.h"

void FlashInit(void);
void FlashWrite(uint8_t * pBuffer, uint8_t size);

#endif	// End of  #ifndef _FLASH_H_void


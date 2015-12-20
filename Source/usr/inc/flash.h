#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm8l15x.h"

void FlashInit(void);
uint8_t DataMem_Write(uint32_t addr, uint8_t * pu8Buffer, uint8_t size);
uint8_t DataMem_SetRemainGas(uint32_t tmp);
uint32_t DataMem_GetRemainGas(void);


#endif	// End of  #ifndef _FLASH_H_void


#ifndef _FLASH_H_
#define _FLASH_H_

#include "stm8l15x.h"

#define DATAMEM_TOP_GAS_START_ADDR			(0x00)
#define DATAMEM_TOP_GAS_OFFSET    			(4)
#define DATAMEM_REAL_GAS_START_ADDR			(0x04)
#define DATAMEM_REAL_GAS_OFFSET    			(4)




void FlashInit(void);
uint8_t DataMem_Write(uint32_t addr, uint8_t * pu8Buffer, uint8_t size);
uint8_t DataMem_SetTopGas(uint32_t tmp);
uint32_t DataMem_GetTopGas(void);
uint8_t DataMem_SetRealGas(uint32_t tmp);
uint32_t DataMem_GetRealGas(void);
uint32_t DataMem_GetRemainGas(void);


#endif	// End of  #ifndef _FLASH_H_void


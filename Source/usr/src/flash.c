#include "flash.h"
#include "stm8l15x.h"
#include <stdio.h>
#include "includes.h"

void FlashInit(void)
{
	// Define flash programming Time
  	FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
}

uint8_t DataMem_Write(uint32_t addr, uint8_t * pu8Buffer, uint8_t size)
{
	uint32_t timeout = 10000;
	uint8_t i = 0;
	uint8_t state = 1;

	usr_assert(pu8Buffer != NULL);

	// Unlock flash data eeprom memory 
  	FLASH_Unlock(FLASH_MemType_Data);
	
  	// Wait until Data EEPROM area unlocked flag is set
  	while ((FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET) && (timeout--))
  	{}

	if(timeout)
	{
		for(i = 0;i < size;i ++)
		{
			FLASH_ProgramByte(addr + i, *(pu8Buffer + i));
		}
		state = 1;
	}
	else
	{
		state = 0;
	}

	// Lock flash data eeprom memory 
  	FLASH_Lock(FLASH_MemType_Data);

	return (state);
}

/**
  * @brief  Reads one byte from flash memory
  * @param  Address : Address to read
  * @retval Value of the byte
  */
uint8_t DataMem_ReadByte(uint32_t Address)
{
  	uint8_t tmp = 0;

	usr_assert((Address >= FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS) && 
				(Address <= FLASH_DATA_EEPROM_END_PHYSICAL_ADDRESS));
  
  	tmp = *(PointerAttr uint8_t *) (MemoryAddressCast)Address;

  	return tmp;
}

uint8_t DataMem_SetRemainGas(uint32_t tmp)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS;
	uint8_t u8arr[4];
	uint8_t state = 1;

	u8arr[0] = (uint8_t)tmp;
	u8arr[1] = (uint8_t)(tmp >> 8);
	u8arr[2] = (uint8_t)(tmp >> 16);
	u8arr[3] = (uint8_t)(tmp >> 24);
	state = DataMem_Write(start_addr, u8arr, sizeof(uint32_t));
	
	return (state);
}

uint32_t DataMem_GetRemainGas(void)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS;
	uint8_t u8Arr[4] = {0};
	uint32_t temp;

	for(uint8_t i = 0;i < sizeof(uint32_t);i ++)
	{
		u8Arr[i] = DataMem_ReadByte(start_addr + i);
	}

	temp = (uint32_t)u8Arr[0] | ((uint32_t)u8Arr[1] << 8) | ((uint32_t)u8Arr[2] << 16) | ((uint32_t)u8Arr[3] << 24);

	return (temp);
}






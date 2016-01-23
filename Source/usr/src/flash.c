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

uint8_t DataMem_SetTopGas(uint32_t tmp)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + DATAMEM_TOP_GAS_START_ADDR;
	uint32_t_or_u8_u temp;
	uint8_t state = 0;

	#ifdef DATAMEM_DEBUG
		printf("[DATAMEM] DataMem_SetTopGas\r\n");
	#endif

	temp.l = tmp;
	ConvertU32Endian(&temp.l);
	state = DataMem_Write(start_addr, temp.arr, DATAMEM_TOP_GAS_OFFSET);

	return (state);
}

uint8_t DataMem_SetRealGas(uint32_t tmp)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + DATAMEM_REAL_GAS_START_ADDR;
	uint32_t_or_u8_u temp;
	uint8_t state = 0;

	#ifdef DATAMEM_DEBUG
		printf("[DATAMEM] DataMem_SetRealGas\r\n");
	#endif

	temp.l = tmp;
	ConvertU32Endian(&temp.l);
	state = DataMem_Write(start_addr, temp.arr, DATAMEM_REAL_GAS_OFFSET);

	return (state);
}

uint32_t DataMem_GetTopGas(void)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + DATAMEM_TOP_GAS_START_ADDR;
	uint32_t_or_u8_u temp;

	#ifdef DATAMEM_DEBUG
		printf("[DATAMEM] DataMem_GetTopGas\r\n");
	#endif

	for(uint8_t i = 0;i < DATAMEM_TOP_GAS_OFFSET;i ++)
	{
		temp.arr[i] = DataMem_ReadByte(start_addr + i);
	}

	#ifdef DATAMEM_DEBUG
		for(uint8_t i = 0;i < DATAMEM_TOP_GAS_OFFSET;i ++)
		{
			printf("0x%02x, ",temp.arr[i]);
		}
		printf("\r\n");
	#endif

	ConvertU32Endian(&temp.l);

	return (temp.l);
}

uint32_t DataMem_GetRealGas(void)
{
	uint32_t start_addr = FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + DATAMEM_REAL_GAS_START_ADDR;
	uint32_t_or_u8_u temp;

	#ifdef DATAMEM_DEBUG
		printf("[DATAMEM] DataMem_GetRealGas\r\n");
	#endif

	for(uint8_t i = 0;i < DATAMEM_TOP_GAS_OFFSET;i ++)
	{
		temp.arr[i] = DataMem_ReadByte(start_addr + i);
	}

	#ifdef DATAMEM_DEBUG
		for(uint8_t i = 0;i < DATAMEM_TOP_GAS_OFFSET;i ++)
		{
			printf("0x%02x, ",temp.arr[i]);
		}
		printf("\r\n");
	#endif

	ConvertU32Endian(&temp.l);
	
	return (temp.l);
}

uint32_t DataMem_GetRemainGas(void)
{
	uint32_t real_gas = 0;
	uint32_t top_gas = 0;
	uint32_t remain_gas = 0;

	#ifdef DATAMEM_DEBUG
		printf("[DATAMEM] DataMem_GetRemainGas\r\n");
	#endif

	real_gas = DataMem_GetRealGas();
	top_gas = DataMem_GetTopGas();

	if(real_gas < top_gas)
	{
		remain_gas = top_gas - real_gas;
	}
	else
	{
		remain_gas = 0;
	}

	return (remain_gas);
}



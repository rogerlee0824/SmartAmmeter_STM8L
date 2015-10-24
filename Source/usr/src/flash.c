#include "flash.h"
#include "stm8l15x.h"

void FlashInit(void)
{
	// Define flash programming Time
  	FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);

  	FLASH_Unlock(FLASH_MemType_Program);
  	// Wait until Flash Program area unlocked flag is set
  	while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
  	{}

  	// Unlock flash data eeprom memory 
  	FLASH_Unlock(FLASH_MemType_Program);
	
  	// Wait until Data EEPROM area unlocked flag is set
  	while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET)
  	{}
}

void FlashWrite(uint8_t * pBuffer, uint8_t size)
{
  	// This function is executed from RAM 
  	FLASH_ProgramBlock(0, FLASH_MemType_Program, FLASH_ProgramMode_Standard, pBuffer);
  
  	// Wait until End of high voltage flag is set
  	while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET)
  	{}
}


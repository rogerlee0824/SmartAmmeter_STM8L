#include "dataConvert.h"
#include "includes.h"
#include <stdio.h>


void Covertu32To6bitBCD(uint32_t u32Data, uint8_t *arr)
{
	usr_assert(arr != NULL);

	arr[0] = u32Data % 1000000 / 100000;
	arr[1] = u32Data % 100000 / 10000;
	arr[2] = u32Data % 10000 / 1000;
	arr[3] = u32Data % 1000 / 100;
	arr[4] = u32Data % 100 / 10;
	arr[5] = (u32Data % 10);
}

uint32_t ConvertU8ToU32(uint8_t * p_data)
{
	uint32_t temp_u32 = 0;
	
	/* Check parameters */
	usr_assert(p_data != NULL);

	temp_u32 = ((((uint32_t)p_data[0]) << 24) |
				(((uint32_t)p_data[1]) << 16) |
				(((uint32_t)p_data[2]) << 8) |
				((uint32_t)p_data[3]));

	return temp_u32;
}


void ConvertU32Endian(uint32_t *p_u32data)
{
	uint32_t tmp = 0;
	
	/* Check parameters */
	usr_assert(p_u32data != NULL);

	tmp = *p_u32data;

	*p_u32data = ((((uint32_t)(tmp) & 0xff000000) >> 24) | 
					(((uint32_t)(tmp) & 0x00ff0000) >> 8) | 
					(((uint32_t)(tmp) & 0x0000ff00) << 8) | 
					(((uint32_t)(tmp) & 0x000000ff) << 24));
}


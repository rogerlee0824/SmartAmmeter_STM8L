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

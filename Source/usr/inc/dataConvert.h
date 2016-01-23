#ifndef _DATACONVERT_H_
#define _DATACONVERT_H_

#include "stm8l15x.h"

void Covertu32To6bitBCD(uint32_t u32Data, uint8_t *arr);

#define ConvertU16Endian(X)                 ((((uint16_t)(X) & 0xff00) >> 8) |(((uint16_t)(X) & 0x00ff) << 8))
//#define ConvertU32Endian(X)                 ((((uint32_t)(X) & 0xff000000) >> 24) | (((uint32_t)(X) & 0x00ff0000) >> 8) | (((uint32_t)(X) & 0x0000ff00) << 8) | (((uint32_t)(X) & 0x000000ff) << 24))

uint32_t ConvertU8ToU32(uint8_t * p_data);
void ConvertU32Endian(uint32_t *p_u32data);


#endif


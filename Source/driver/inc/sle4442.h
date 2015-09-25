#ifndef _AT88SC102_H_
#define _AT88SC102_H_

#include "stm8l15x.h"

#define RMM_COMM 				0x30    //¶ÁÖ÷´æÃüÁî×Ö
#define WMM_COMM 				0x38    //Ğ´Ö÷´æÃüÁî×Ö
#define VER_COMM 				0x33    //Ğ£ÑéÃÜÂë
#define RSM_COMM 				0x31    //¶ÁÃÜÂë´æ´¢Çø
#define WSM_COMM 				0x39    //Ğ´ÃÜÂë´æ´¢Çø
#define RPM_COMM 				0x34    //¶Á±£»¤´æ´¢Çø
#define WPM_COMM 				0x3c    //Ğ´±£»¤´æ´¢Çø

void SLE4442_Init(void);
void SLE4442_Reset(void);
void SLE4442_Break(void);
uint8_t SLE4442_I2C_ReadByte(void);
void SLE4442_I2C_Read(uint8 * pDstBuffer, uint8_t len);
void SLE4442_Write(uint8_t * pDstBuffer, uint8_t len);
void SLE4442_I2C_WriteByte(uint8 * pSrcBuffer);
void SLE4442_WriteMainMem(uint8_t addr, uint8_t *pt);
void SLE4442_ReadMainMem(uint8_t addr, uint8_t *pt,uint8_t count);
void SLE4442_ProtectByte(uint8_t addr,uint8_t *pt);
uint8_t SLE4442_Verify(uint8_t *pt);

#endif
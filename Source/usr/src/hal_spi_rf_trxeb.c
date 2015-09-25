/******************************************************************************
*  Filename: hal_spi_rf_trxeb.c
*
*  Description: Implementation file for common spi access with the CCxxxx 
*               tranceiver radios using trxeb. Supports CC1101/CC112X radios
*
*  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
*
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/


/******************************************************************************
 * INCLUDES
 */
#include "includes.h"
#include "stm8l15x_clk.h"
#include "stm8l15x_gpio.h"
#include "stm8l15x_spi.h"
#include "hal_spi_rf_trxeb.h"

/******************************************************************************
 * LOCAL FUNCTIONS
 */
static void trxReadWriteBurstSingle(uint8_t addr,uint8_t *pData,uint16_t len) ;
uint8_t CC112x_SPIReadWriteByte(SPI_TypeDef* SPIx,uint8_t temp);

/******************************************************************************
 * FUNCTIONS
 */

/******************************************************************************
 * @fn          trxRfSpiInterfaceInit
 *
 * @brief       Function to initialize TRX SPI. CC1101/CC112x is currently
 *              supported. The supported prescalerValue must be set so that
 *              SMCLK/prescalerValue does not violate radio SPI constraints.
 *
 * input parameters
 *
 * @param       prescalerValue - SMCLK/prescalerValue gives SCLK frequency
 *
 * output parameters
 *
 * @return      void
 */
void trxRfSpiInterfaceInit(void)
{

  /* Configuration
   * -  8-bit
   * -  Master Mode
   * -  3-pin
   * -  synchronous mode
   * -  MSB first
   * -  Clock phase select = captured on first edge
   * -  Inactive state is low
   * -  SMCLK as clock source
   * -  Spi clk is adjusted corresponding to systemClock as the highest rate
   *    supported by the supported radios: this could be optimized and done
   *    after chip detect.
   */
   	/* CC1120_SPI Periph clock enable */
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);

  	/* Set the MOSI,MISO and SCK at high level */
  	GPIO_ExternalPullUpConfig(GPIO_PORT_CC112X_SPI_SCK, GPIO_PIN_CC112X_SPI_SCK, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_CC112X_SPI_MOSI, GPIO_PIN_CC112X_SPI_MOSI, ENABLE);
	GPIO_ExternalPullUpConfig(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO, ENABLE);


	/* Configure FLASH_CS as Output push-pull, used as Flash Chip select */
	GPIO_Init(GPIO_PORT_CC112X_SPI_NSS, GPIO_PIN_CC112X_SPI_NSS, GPIO_Mode_Out_PP_High_Slow);
	GPIO_SetBits(GPIO_PORT_CC112X_SPI_NSS, GPIO_PIN_CC112X_SPI_NSS);
	
	/* SPI configuration */
	SPI_Init(APP_CC112X_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
	
	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, ENABLE);

}

/*******************************************************************************
* @brief  Sends a byte through the SPI interface and return the byte
* @param  SPIx: To select the SPIx/I2Sx peripheral
* @param  temp: Data to be transmitted.
* @retval The value of the received data.
*******************************************************************************/
uint8_t CC112x_SPIReadWriteByte(SPI_TypeDef* SPIx,uint8_t temp)
{
    uint16_t wr_timeout = 1000,rd_timeout = 4000;

    /* Send byte through the SPIx peripheral */
    SPI_SendData(SPIx,temp);
    
	/* Loop while DR register in not emplty */
    while ((SPI_GetFlagStatus(SPIx, SPI_FLAG_TXE) == RESET) && (--wr_timeout));
    if(!wr_timeout)
    {
		#ifdef CC112x_DEBUG
	    	printf("[cc112x] write timeout.\r\n");
		#endif    
    }

    /* Wait to receive a byte */
    while ((SPI_GetFlagStatus(SPIx, SPI_FLAG_RXNE) == RESET) && (--rd_timeout));
    if(!rd_timeout)
    {
		#ifdef CC112x_DEBUG
        	printf("[cc112x] read timeout.\r\n");
		#endif    
    }  
    
    /* Return the byte read from the SPI bus */
    return (SPI_ReceiveData(SPIx));
}

/*******************************************************************************
 * @fn          trx8BitRegAccess
 *
 * @brief       This function performs a read or write from/to a 8bit register
 *              address space. The function handles burst and single read/write
 *              as specfied in addrByte. Function assumes that chip is ready.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       addrByte - address byte of register.
 * @param       pData    - data array
 * @param       len      - Length of array to be read(TX)/written(RX)
 *
 * output parameters
 *
 * @return      chip status
 */
rfStatus_t trx8BitRegAccess(uint8_t accessType, uint8_t addrByte, uint8_t *pData, uint16_t len)
{
  uint8_t readValue;

	#ifdef CC112x_DEBUG
    	//printf("[cc112x] trx8BitRegAccess.\r\n");
	#endif  
	/* SPI configuration */
	SPI_Init(APP_CC112X_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
	
	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, ENABLE);
	
  /* Pull CS_N low and wait for SO to go low before communication starts */
  TRXEM_SPI_BEGIN();
  while(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO) == SET);

  /* send register address byte */
  readValue = CC112x_SPIReadWriteByte(APP_CC112X_SPI, (accessType|addrByte));
  
  trxReadWriteBurstSingle(accessType|addrByte,pData,len);
  TRXEM_SPI_END();

  /* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, DISABLE);
  
  /* return the status byte value */
  return(readValue);
}

/******************************************************************************
 * @fn          trx16BitRegAccess
 *
 * @brief       This function performs a read or write in the extended adress
 *              space of CC112X.
 *
 * input parameters
 *
 * @param       accessType - Specifies if this is a read or write and if it's
 *                           a single or burst access. Bitmask made up of
 *                           RADIO_BURST_ACCESS/RADIO_SINGLE_ACCESS/
 *                           RADIO_WRITE_ACCESS/RADIO_READ_ACCESS.
 * @param       extAddr - Extended register space address = 0x2F.
 * @param       regAddr - Register address in the extended address space.
 * @param       *pData  - Pointer to data array for communication
 * @param       len     - Length of bytes to be read/written from/to radio
 *
 * output parameters
 *
 * @return      rfStatus_t
 */
rfStatus_t trx16BitRegAccess(uint8_t accessType, uint8_t extAddr, uint8_t regAddr, uint8_t *pData, uint8_t len)
{
  uint8_t readValue;

	#ifdef CC112x_DEBUG
    	//printf("[cc112x] trx16BitRegAccess.\r\n");
	#endif

	/* SPI configuration */
	SPI_Init(APP_CC112X_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);
	
	/* Enable SPI  */
  SPI_Cmd(APP_CC112X_SPI, ENABLE);
	
  TRXEM_SPI_BEGIN();
  while(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO) == SET);
  
  /* send extended address byte with access type bits set */
  readValue = CC112x_SPIReadWriteByte(APP_CC112X_SPI, (accessType|extAddr));
  CC112x_SPIReadWriteByte(APP_CC112X_SPI, regAddr);
  
  /* Communicate len number of bytes */
  trxReadWriteBurstSingle(accessType|extAddr,pData,len);
  TRXEM_SPI_END();

  /* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, DISABLE);
  
  /* return the status byte value */
  return(readValue);
}

/*******************************************************************************
 * @fn          trxSpiCmdStrobe
 *
 * @brief       Send command strobe to the radio. Returns status byte read
 *              during transfer of command strobe. Validation of provided
 *              is not done. Function assumes chip is ready.
 *
 * input parameters
 *
 * @param       cmd - command strobe
 *
 * output parameters
 *
 * @return      status byte
 */
rfStatus_t trxSpiCmdStrobe(uint8_t cmd)
{
    uint8_t rc;

	/* SPI configuration */
	SPI_Init(APP_CC112X_SPI, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,
           SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_Direction_2Lines_FullDuplex,
           SPI_NSS_Soft, 0x07);

	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, ENABLE);
    TRXEM_SPI_BEGIN();
    while(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO) == SET);

	rc = CC112x_SPIReadWriteByte(APP_CC112X_SPI, cmd);
	
    TRXEM_SPI_END();
	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, DISABLE);
	
    return(rc);
}


/*******************************************************************************
 * @fn          trxReadWriteBurstSingle
 *
 * @brief       When the address byte is sent to the SPI slave, the next byte
 *              communicated is the data to be written or read. The address
 *              byte that holds information about read/write -and single/
 *              burst-access is provided to this function.
 *
 *              Depending on these two bits this function will write len bytes to
 *              the radio in burst mode or read len bytes from the radio in burst
 *              mode if the burst bit is set. If the burst bit is not set, only
 *              one data byte is communicated.
 *
 *              NOTE: This function is used in the following way:
 *
 *              TRXEM_SPI_BEGIN();
 *              while(TRXEM_PORT_IN & TRXEM_SPI_MISO_PIN);
 *              ...[Depending on type of register access]
 *              trxReadWriteBurstSingle(uint8 addr,uint8 *pData,uint16 len);
 *              TRXEM_SPI_END();
 *
 * input parameters
 *
 * @param       none
 *
 * output parameters
 *
 * @return      void
 */
static void trxReadWriteBurstSingle(uint8_t addr,uint8_t *pData,uint16_t len)
{
	uint16_t i;

	//usr_assert(pData != NULL);
	//usr_assert(len > 0);

	#ifdef CC112x_DEBUG
	   // printf("[cc112x] rxReadWriteBurstSingle...\r\n");   
	#endif
			
  /* Communicate len number of bytes: if RX - the procedure sends 0x00 to push bytes from slave*/
  if(addr&RADIO_READ_ACCESS)
  {
    if(addr&RADIO_BURST_ACCESS)
    {
      for (i = 0; i < len; i++)
      {
		  *pData++ = CC112x_SPIReadWriteByte(APP_CC112X_SPI, 0);
      }
    }
    else
    {
	  *pData = CC112x_SPIReadWriteByte(APP_CC112X_SPI, 0);
    }
  }
  else
  {
    if(addr&RADIO_BURST_ACCESS)
    {
      /* Communicate len number of bytes: if TX - the procedure doesn't overwrite pData */
      for (i = 0; i < len; i++)
      {
		CC112x_SPIReadWriteByte(APP_CC112X_SPI, *pData++);
      }
    }
    else
    {
      CC112x_SPIReadWriteByte(APP_CC112X_SPI, *pData);
    }
  }
  return;
}
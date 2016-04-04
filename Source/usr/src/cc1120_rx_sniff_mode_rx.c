//******************************************************************************
//! @file       cc1120_rx_sniff_mode_rx.c
//! @brief      This program demonstrates Rx Sniff Mode for the 1.2 kbps,
//              50 kHz RX filter BW setting available in SmartRf Studio.
//              The transmitter sends a packet every time a button is pushed and
//              the receiver implements RX Sniff Mode to reduce the current
//              consumption.
//              DN511 (http://www.ti.com/lit/swra428) explains how the register
//              settings are found.
//              Note! It is important that RX and TX configurations matches:
//              RX (Config 1) - TX (Config 1)
//              RX (Config 2) - TX (Config 2 and 3)
//              RX (Config 3) - TX (Config 2 and 3)
//
//  Copyright (C) 2013 Texas Instruments Incorporated - http://www.ti.com/
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//      Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//      Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//      Neither the name of Texas Instruments Incorporated nor the names of
//      its contributors may be used to endorse or promote products derived
//      from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************/


/*******************************************************************************
* INCLUDES
*/

#include "cc112x.h"
#include "includes.h"
#include "cc1120_sniff_mode.h"
#include "delay.h"
#include <stdio.h>

/*******************************************************************************
* LOCAL VARIABLES
*/

uint8_t  TXpacketSemaphore;
uint8_t  RXpacketSemaphore;

/*******************************************************************************
* STATIC FUNCTIONS
*/
void registerConfig(void);
void manualCalibration(void);
void calibrateRCOsc(void);
uint8_t ReadDataFromRadio(uint8_t *rxBuffer);

void rf_PowerUpReset(void)
{
    #ifdef CC112x_DEBUG
         printf("[CC112X] rf_PowerUpReset...\r\n"); 
	#endif
         
    GPIO_ResetBits(GPIO_PORT_CC112X_SPI_NSS,GPIO_PIN_CC112X_SPI_NSS);       // Pulse CSn low then high 
    delay1us(20); 								                    		// Delay 20us 
    GPIO_SetBits(GPIO_PORT_CC112X_SPI_NSS,GPIO_PIN_CC112X_SPI_NSS);         // Pulse CSn high 
    delay1us(60); 								                    		// Delay 60us 

	// pull CSn low and wait for SO to go low 
    /* Pull CS_N low and wait for SO to go low before communication starts */
	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, ENABLE);
    GPIO_ResetBits(GPIO_PORT_CC112X_SPI_NSS,GPIO_PIN_CC112X_SPI_NSS);       // CS = 0
    while(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO) == SET);
    
    cc112xSpiWriteReg(0x00, 0x00,1); 							// SPI写字节，不是写命令子程序 
    
    // wait for SO to go low again, reset is complete at that point 
    while(GPIO_ReadInputDataBit(GPIO_PORT_CC112X_SPI_MISO, GPIO_PIN_CC112X_SPI_MISO) == SET);
    GPIO_SetBits(GPIO_PORT_CC112X_SPI_NSS,GPIO_PIN_CC112X_SPI_NSS);       // Pulse CSn high

	/* Enable SPI  */
    SPI_Cmd(APP_CC112X_SPI, DISABLE);
    #ifdef CC112x_DEBUG
         printf("[CC112X] rf_PowerUpReset,OK...\r\n"); 
	#endif
}

uint8_t ReceivePacketByRF(uint8_t *rxBuffer) 
{
  	uint8_t rxBytes;
  	uint8_t marcState;
	
  	// Set radio in RX
  	trxSpiCmdStrobe(CC112X_SRX);
	
	// Wait for packet received interrupt 
	while(RXpacketSemaphore != ISR_ACTION_REQUIRED);
	
	// Reset packet semaphore
    RXpacketSemaphore = ISR_IDLE;
			
	// Read number of bytes in RX FIFO
	cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);
      
	// Check that we have bytes in FIFO
    if(rxBytes != 0) 
	{
      	// Read marcstate to check for RX FIFO error
		cc112xSpiReadReg(CC112X_MARCSTATE, &marcState, 1);

//        // Mask out marcstate bits and check if we have a RX FIFO error
//        if((marcState & 0x1F) == RX_FIFO_ERROR) 
//		{
//			// Flush RX FIFO
//			trxSpiCmdStrobe(CC112X_SFRX);
//					
//			return 0;
//		} 
//		else 
//		{
//			// Read n bytes from RX FIFO
//          	cc112xSpiReadRxFifo(rxBuffer, rxBytes);  
//          
//          	// Check CRC ok (CRC_OK: bit7 in second status byte)
//          	// This assumes status bytes are appended in RX_FIFO
//          	// (PKT_CFG1.APPEND_STATUS = 1.)
//          	// If CRC is disabled the CRC_OK field will read 1
//          	if(rxBuffer[rxBytes - 1] & 0x80) 
//		  	{
//            	// Update packet counter
//            	packetCounter++;
//          	}
//					
//			return rxBytes;
//        }
	} 
	
	return 0;
}

void SendPacketByRF(uint8_t *u8pData,uint8_t u8Len) 
{
    uint8_t marcstate;
	// Write packet to TX FIFO
    trxSpiCmdStrobe(CC112X_SFTX);
    trxSpiCmdStrobe(CC112X_SFSTXON);
    cc112xSpiWriteTxFifo(u8pData, u8Len);
	// Strobe TX to send packet
    trxSpiCmdStrobe(CC112X_STX);
	// Wait for interrupt that packet has been sent. 
	// (Assumes the GPIO connected to the radioRxTxISR function is set 
	// to GPIOx_CFG = 0x06)
    // while(TXpacketSemaphore != ISR_ACTION_REQUIRED);
    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
    } while (marcstate != 0x41);
	// Clear semaphore flag
    TXpacketSemaphore = ISR_IDLE;
}
/*******************************************************************************
*   @fn         ReadDataFromRadio
*   @brief      Read from FIFO in the Radio
*   @param      The saved buffer
*   @return     The number to read out
*******************************************************************************/
uint8_t ReadDataFromRadio(uint8_t *rxBuffer)
{
    uint8_t rxBytes;
    //RXpacketSemaphore = ISR_IDLE;
    cc112xSpiReadReg(CC112X_NUM_RXBYTES, &rxBytes, 1);
    if(rxBytes != 0)
    {
        cc112xSpiReadRxFifo(rxBuffer, rxBytes);
    }
    trxSpiCmdStrobe(CC112X_SIDLE);
    trxSpiCmdStrobe(CC112X_SFRX);
    RXpacketSemaphore = ISR_IDLE; 
    return rxBytes;
}

/*******************************************************************************
*   @fn         calibrateRcOsc
*   @brief      Calibrates the RC oscillator used for the eWOR timer. When this
*               function is called, WOR_CFG0.RC_PD must be 0
*   @param      none
*   @return     none
*******************************************************************************/
void calibrateRCOsc(void) 
{
    uint8_t temp;

    // Read current register value
    cc112xSpiReadReg(CC112X_WOR_CFG0, &temp,1);

    // Mask register bit fields and write new values
    temp = (temp & 0xF9) | (0x02 << 1);

    // Write new register value
    cc112xSpiWriteReg(CC112X_WOR_CFG0, &temp,1);

    // Strobe IDLE to calibrate the RCOSC
    trxSpiCmdStrobe(CC112X_SIDLE);

    // Disable RC calibration
    temp = (temp & 0xF9) | (0x00 << 1);
    cc112xSpiWriteReg(CC112X_WOR_CFG0, &temp, 1);
}

/*******************************************************************************
*   @fn         registerConfig
*   @brief      Write register settings as given by SmartRF Studio found in
*               cc1120_rx_sniff_mode_reg_config.h
*   @param      none
*   @return     none
*******************************************************************************/
void registerConfig(void) 
{
    uint8_t writeByte = 0;
    uint16_t i;
	uint8_t readByte = 0;
    
    // Reset radio
    trxSpiCmdStrobe(CC112X_SRES);

    // Write registers to radio
    for(i = 0;i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) 
	{
        writeByte = preferredSettings[i].data;       
        cc112xSpiWriteReg(preferredSettings[i].addr, &writeByte, 1);
        delay1us(1000);
    }

    #ifdef CC112x_DEBUG
       printf("[CC112X] registerConfig,OK...\r\n");  
		for(i = 0;i < (sizeof(preferredSettings)/sizeof(registerSetting_t)); i++) 
		{
			cc112xSpiReadReg(preferredSettings[i].addr, &readByte, 1);
			printf("[CC112X] i = %d : 0x%02x \r\n",i,readByte); 
		}
    #endif	

	// Calibrate radio according to errata
    manualCalibration();
    
      
}

/*******************************************************************************
*   @fn         manualCalibration
*   @brief      Calibrates radio according to CC112x errata
*   @param      none
*   @return     none
*******************************************************************************/
#define VCDAC_START_OFFSET 2
#define FS_VCO2_INDEX 0
#define FS_VCO4_INDEX 1
#define FS_CHP_INDEX 2
void manualCalibration(void)
{
    uint8_t original_fs_cal2;
    uint8_t calResults_for_vcdac_start_high[3];
    uint8_t calResults_for_vcdac_start_mid[3];
    uint8_t marcstate;
    uint8_t writeByte;

    // 1) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 2) Start with high VCDAC (original VCDAC_START + 2):
    cc112xSpiReadReg(CC112X_FS_CAL2, &original_fs_cal2, 1);
    writeByte = original_fs_cal2 + VCDAC_START_OFFSET;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 3) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
    } while (marcstate != 0x41);

    // 4) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained with 
    //    high VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2,
                     &calResults_for_vcdac_start_high[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_high[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_high[FS_CHP_INDEX], 1);

    // 5) Set VCO cap-array to 0 (FS_VCO2 = 0x00)
    writeByte = 0x00;
    cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);

    // 6) Continue with mid VCDAC (original VCDAC_START):
    writeByte = original_fs_cal2;
    cc112xSpiWriteReg(CC112X_FS_CAL2, &writeByte, 1);

    // 7) Calibrate and wait for calibration to be done
    //   (radio back in IDLE state)
    trxSpiCmdStrobe(CC112X_SCAL);

    do {
        cc112xSpiReadReg(CC112X_MARCSTATE, &marcstate, 1);
    } while (marcstate != 0x41);

    // 8) Read FS_VCO2, FS_VCO4 and FS_CHP register obtained 
    //    with mid VCDAC_START value
    cc112xSpiReadReg(CC112X_FS_VCO2, 
                     &calResults_for_vcdac_start_mid[FS_VCO2_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_VCO4,
                     &calResults_for_vcdac_start_mid[FS_VCO4_INDEX], 1);
    cc112xSpiReadReg(CC112X_FS_CHP,
                     &calResults_for_vcdac_start_mid[FS_CHP_INDEX], 1);

    // 9) Write back highest FS_VCO2 and corresponding FS_VCO
    //    and FS_CHP result
    if (calResults_for_vcdac_start_high[FS_VCO2_INDEX] >
        calResults_for_vcdac_start_mid[FS_VCO2_INDEX]) 
	{
        writeByte = calResults_for_vcdac_start_high[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_high[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    } 
	else 
	{
        writeByte = calResults_for_vcdac_start_mid[FS_VCO2_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO2, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_VCO4_INDEX];
        cc112xSpiWriteReg(CC112X_FS_VCO4, &writeByte, 1);
        writeByte = calResults_for_vcdac_start_mid[FS_CHP_INDEX];
        cc112xSpiWriteReg(CC112X_FS_CHP, &writeByte, 1);
    }
}

void worRssiInit(void)
{ 
    uint8_t writeByte;
    // Set carrier sense gate mode on recevier, and set AGC carrier sense threshold
    writeByte = 0x37; //approx -40 dBm
    cc112xSpiWriteReg(CC112X_AGC_CS_THR,&writeByte,1);

    // Set RSSI valid count to minimum: 2 COMPUTATIONS
    writeByte = 0xC3;
    cc112xSpiWriteReg(CC112X_AGC_CFG0,&writeByte,1);

    //Set AGC window size to 8 samples
    //Set AGC settle wait time to 24 SAMPLES
    writeByte = 0xA0;
    cc112xSpiWriteReg(CC112X_AGC_CFG1,&writeByte,1);

    // Turning off autocal    
    writeByte = 0x03;
    cc112xSpiWriteReg(CC112X_SETTLING_CFG,&writeByte,1);

    // Setting WOR mode to NORMAL_MODE & WOR timer resolution to HIGH_RESOLUTION
    writeByte = 0x08;
    cc112xSpiWriteReg(CC112X_WOR_CFG0,&writeByte,1);

    //Setting Event0 timeout
    writeByte = 0x02; //20 ms 22 ms 23 ms
    cc112xSpiWriteReg(CC112X_WOR_EVENT0_MSB,&writeByte,1);
    writeByte = 0x80; //20 ms
    cc112xSpiWriteReg(CC112X_WOR_EVENT0_LSB,&writeByte,1);

    //Event1 timeout set to 1 ms to allow XOSC anddigital regulator to startup and settle
    writeByte = 0x0E;
    cc112xSpiWriteReg(CC112X_WOR_CFG1,&writeByte,1);

    //Disable Event2 configuration. RC_OSC calibraton interval disabled.
    //Setting RC osc enable
    //Setting RC calibration mode to continuous calibration  
    writeByte = 0x24;
    cc112xSpiWriteReg(CC112X_WOR_CFG0,&writeByte,1);

    // Strobe idle to calibrate
    trxSpiCmdStrobe(CC112X_SIDLE);
       
    //disable RC osc callibration
    writeByte = 0x20;
    cc112xSpiWriteReg(CC112X_WOR_CFG0,&writeByte,1);

    //Setting RX timeout to never 
    //(in case of noise making the radio hang in RX this can be set
    // to force powerdown after a certain time in RX)
    writeByte = 0x0E;
    cc112xSpiWriteReg(CC112X_RFEND_CFG1,&writeByte,1);

    //Setting RX_TERMINATE_ON_RSSI
    writeByte = 0x09;
    cc112xSpiWriteReg(CC112X_RFEND_CFG0,&writeByte,1);
}

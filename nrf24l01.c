/*
 * nrf24l01.c
 *
 *  Created on: Jul 29, 2014
 *      Author: Rohit Dureja
 */

#include "nrf24l01.h"

#define LED_0	GPIO_PIN_0
#define LED_1	GPIO_PIN_1
#define LED_2	GPIO_PIN_2
#define LED_3	GPIO_PIN_3

// initialize the RF module
void RFInit(uint32_t ui32Mode)
{
	SPIInit();

	if(ui32Mode == 0) // RX Mode
	{
		SPISetCELow();

		// ----- //
		SPISetCSNLow();
		RFWriteRegister(WRITE_REG + SETUP_AW, 0x01); // Set address width to three bytes
		// set RX pipe 0 address
		SPIDataWrite(WRITE_REG + RX_ADDR_P0);
		SPIDataWrite(0x3C); // LSB
		SPIDataWrite(0x3C);
		SPIDataWrite(0x3C); // MSB
		RFWriteRegister(WRITE_REG + EN_AA, 0x01); // enable ACK on RX pipe 0
		RFWriteRegister(WRITE_REG + EN_RXADDR, 0x01); // enable data pipe 0
		RFWriteRegister(WRITE_REG + RF_CH, 60); // set RF channel
		RFWriteRegister(WRITE_REG + RF_SETUP, 0x0E); // set data rate at 2mbps and power at 0dBm
		RFWriteRegister(WRITE_REG + DYNPD, 0x01); // enable dynamic payload length for RX pipe 0
		RFWriteRegister(WRITE_REG + FEATURE, 0x04); // enable dynamic payload length
		RFWriteRegister(WRITE_REG + CONFIG, 0x3F); // RX_DR interrupt on IRQ pin and RX mode on
		SPISetCSNHigh();
		// Flush SPI RX FIFO to remove residual data
		SPIRXFlush();
		// ----- //

		SPISetCEHigh();
	}
	else if(ui32Mode == 1) // TX Mode
	{
		SPISetCELow();

		// ----- //
		SPISetCSNLow();
		// ----- //

		SPISetCEHigh();
	}
}

// write into a register
uint32_t RFWriteRegister(uint32_t ui32Register, uint32_t ui32Value)
{
	uint32_t ui32Status;
	SPISetCSNLow();
	SPIDataWrite(ui32Register);
	ui32Status = SPIDataRead();
	SPIDataWrite(ui32Value);
	SPIDataRead();
	SPISetCSNHigh();
	return ui32Status;
}

// read from a RF register
uint32_t RFReadRegister(uint32_t ui32Register)
{
	uint32_t ui32Value;
	SPISetCSNLow();
	SPIDataWrite(ui32Register);
	ui32Value = SPIDataRead();
	SPIDataWrite(0xFF);
	ui32Value = SPIDataRead();
	SPISetCSNHigh();
	return ui32Value;
}

// write to send buffer. Returns numbers of bytes written
uint32_t RFWriteSendBuffer(uint32_t *ui32Data, uint32_t ui32Bytes)
{
	uint32_t i;

	//Flush TX buffer
	SPISetCSNLow();
	SPIDataWrite(FLUSH_TX);
	SPISetCSNHigh();

	SPISetCSNLow();
	SPIDataWrite(WR_TX_PLOAD);
	for(i = 0 ; i < ui32Bytes ; ++i)
	{
		SPIDataWrite(ui32Data[i]);
	}
	SPISetCSNHigh();
	return i;
}

// read from recive buffer. Returns number of bytes read
uint32_t RFReadRecieveBuffer(uint32_t *ui32Data)
{
	uint32_t ui32Bytes;
	uint32_t i;
	// Find number of bytes to read
	SPISetCSNLow();
	SPIDataWrite(R_RX_PL_WID);
	ui32Bytes = SPIDataRead();
	SPIDataWrite(0xFF);
	ui32Bytes = SPIDataRead();
	SPISetCSNHigh();

	// if bytes > 32. Some error has occurred.
	if(ui32Bytes > 32)
	{
		// Flush RX FIFO
		SPISetCSNLow();
		SPIDataWrite(FLUSH_RX);
		SPISetCSNHigh();
		return 0;
	}
	else
	{
		SPISetCSNLow();
		SPIDataWrite(RD_RX_PLOAD);
		SPIDataRead(); // first bytes not important contains status
		for(i = 0 ; i < ui32Bytes ; ++i)
		{
			SPIDataWrite(0xFF);
			ui32Data[i] = SPIDataRead();
		}
		SPISetCSNHigh();
		return ui32Bytes;
	}
}


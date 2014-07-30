Nordic-nRF24L01-RF-Driver
=========================

Driver library for the Nordic nRF24L01+ RF module

File Structure
-----------------

The library is divied into two three layers; application layer (main.c), RF control (nrf24l01.c/.h) and hardware interface (spi.c/.h). 

RF Control Layer
----------------
The RF control layer contains defines for functions which can be used directly in the application code. These functions are:
  
1. RFInit(mode) : this function is used to initialize the RF module for RX/TX operation. If mode is 0, RX operation is enabled and if 1, TX mode is enabled.
  
2. RFWriteRegister(register, value) - this function is used to write data into one of the 8-byte registers in the RF module. Example, to write 0x3F to the CONFIG register, use RFWriteRegister(WRITE_REG + CONFIG, 0x3F.
  
3. RFReadRegister(register) - this function is used to read data from a register in the RF module. It returns the 8-byte data read.
  
4. RFWriteSendBuffer(txbuffer, numBytes) - this function is used only in TX mode and is used to place data contained in the txbuffer array into the TX FIFO of the RF module.
 
5. RFReadRecieveBuffer(rxbuffer) - this function is used only in the RX mode and is used to read data from RX FIFO and store it in the array pointed by rxbuffer. The function returns the number of bytes read.

The RF TX operation can be changed by modifying the RFInit() defined in nrf24l01.c. 

Hardware Interface
-------------------

This layer defines the control structures for communicating with the RF module over SPI. To port the library to any other platform, files of the this layer need to be modified. The defined functions are:

1. SPIInit() - initializes the SPI operation and related GPIO pins and other peripherals.
 
2. SPIDataWrite(byte) - this function writes a byte to the SPI TX FIFO.
 
3. SPIDataRead() - this function reads a byte from the SPI RX FIFO.
 
4. SPISetCSNLow()/SPISetCSNHigh() - these functions control the chip select of the RF module.
 
5. SPISetCELow()/SPISetCEHigh() - these functions control the communication enable of the RF module.

To port the application to another platform, only the spi.c and spi.h components of the library need to be modified. 

 

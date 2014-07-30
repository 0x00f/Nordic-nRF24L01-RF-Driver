Nordic-nRF24L01-RF-Driver
=========================

Driver library for the Nordic nRF24L01+ RF module

File Structure
-----------------

The library is divied into two three layers; application layer (main.c), RF control (nrf24l01.c/.h) and hardware interface (spi.c/.h). 

RF Control Layer
__________________
The RF control layer contains defines for functions which can be used directly in the application code. These functions are:
1. RFInit(mode) : This function is used to initialize the RF module for RX/TX operation. If mode is 0, RX operation is enabled and if 1, TX mode is enabled.
2. RFWriteRegister(register, value) - This function is used to write data into one of the 8-byte registers in the RF module. Example, to write 0x3F to the CONFIG register, use RFWriteRegister(WRITE_REG + CONFIG, 0x3F.
3. RFReadRegister(register) - This function is used to read data from a register in the RF module. It returns the 8-byte data read.
4. RFWriteSendBuffer(txbuffer) - This function is used only in TX mod

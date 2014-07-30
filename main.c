#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "utils/uartstdio.h"
#include "nrf24l01.h"
#include "spi.h"

uint32_t ui32TxBuffer[MAX_PLOAD];
uint32_t ui32RxBuffer[MAX_PLOAD];

// Define pin to LED color mapping.
#define LED_0	GPIO_PIN_0
#define LED_1	GPIO_PIN_1
#define LED_2	GPIO_PIN_2
#define LED_3	GPIO_PIN_3

// The error routine that is called if the driver library encounters an error.
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

void ConfigureUART(void)
{
    // Enable the GPIO Peripheral used by the UART.
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Enable UART0
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UART0);

    // Configure GPIO Pins for UART mode.
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}

void IRQInterruptHandler(void)
{
	uint32_t ui32Bytes, i;
	GPIOIntClear(IRQ_BASE, GPIO_INT_PIN_7); // clear interrupt flag

	SPISetCELow(); // set CE low to cease all operation

//	// --------------- TX operation  ------------- //
//	//Flush TX buffer
//	SPISetCSNLow();
//	SPIDataWrite(FLUSH_TX);
//	SPIDataRead();
//	SPISetCSNHigh();
//
//	RFWriteRegister(WRITE_REG + STATUSREG, 0x10); // Clear MAX_RT flag
//
//	// Do something
//	// Custom Board Board
//	GPIOPinWrite(GPIO_PORTB_BASE, LED_0, 0);
//	SysCtlDelay(SysCtlClockGet()/12);
//	GPIOPinWrite(GPIO_PORTB_BASE, LED_0, LED_0);
//	SysCtlDelay(SysCtlClockGet()/12);
//	UARTprintf("fail\n");
//
//	// Launchpad Board
//	/*GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
//	SysCtlDelay(SysCtlClockGet()/12);
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);
//	SysCtlDelay(SysCtlClockGet()/12);
//	UARTprintf("fail\n");*/
//
//	// --------------- TX operation  ------------- //

	// --------------- RX operation  ------------- //
	ui32Bytes = RFReadRecieveBuffer(ui32RxBuffer);
	UARTprintf("%d received: ", ui32Bytes);
	for(i = 0 ; i < ui32Bytes ; ++i)
	{
		UARTprintf("%x ", ui32RxBuffer[i]);
	}
	UARTprintf("\n");

	// Flush RX buffer
	SPISetCSNLow();
	SPIDataWrite(FLUSH_RX);
	SPIDataRead();
	SPISetCSNHigh();

	RFWriteRegister(WRITE_REG + STATUSREG, 0x40); // Clear RX_DR flag
	// --------------- RX operation  ------------- //

	SPISetCEHigh(); // set CE high again to start all operation
}

// Main 'C' Language entry point.
int main(void)
{
	uint32_t i;

	// Setup the system clock to run at 50 Mhz from PLL with external oscillator
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable and configure the GPIO port for the LED operation.
    // Custom Board Board
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_0 | LED_1 | LED_2 | LED_3 );

    // Launchpad Board
    /*ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);*/

    // --------------- RX operation  ------------- //
    // Initialize RF module port for RX
    RFInit(0);
    // --------------- RX operation  ------------- //

//    // --------------- TX operation  ------------- //
//    // Initialize RF module port for TX
//    RFInit(1);
//    // --------------- TX operation  ------------- //

    // Set up IRQ for handling interrupts
    ROM_GPIOPinTypeGPIOInput(IRQ_BASE, IRQ);
    ROM_GPIOPadConfigSet(IRQ_BASE, IRQ, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntRegister(IRQ_BASE, IRQInterruptHandler);
    ROM_GPIOIntTypeSet(IRQ_BASE, IRQ, GPIO_FALLING_EDGE);
    GPIOIntEnable(IRQ_BASE, GPIO_INT_PIN_7);

    // configure UART for console operation
    ConfigureUART();

//    // --------------- TX operation  ------------- //
//    // Generate packet to send
//    for(i = 1 ; i <= 32 ; ++i)
//    	ui32TxBuffer[i-1] = i;
//    // --------------- TX operation  ------------- //

    // Loop Forever
    while(1)
    {
//    	// --------------- TX operation  ------------- //
//    	// Send packet every one second
//        RFWriteSendBuffer(ui32TxBuffer, 32);
//        ROM_SysCtlDelay(SysCtlClockGet()/3);
//        // --------------- TX operation  ------------- //
    }
}


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/ssi.h"
#include "utils/uartstdio.h"
#include "spi.h"

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

// Main 'C' Language entry point.
int main(void)
{
	// Setup the system clock to run at 50 Mhz from PLL with external oscillator
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Enable and configure the GPIO port for the LED operation.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, LED_0 | LED_1 | LED_2 | LED_3 );

    // Initialize SPI port
    SPIInit();

    ConfigureUART();
    // Loop Forever
    while(1)
    {
    	GPIOPinWrite(GPIO_PORTB_BASE, LED_0, 0);
    	SysCtlDelay(SysCtlClockGet()/12);
    	GPIOPinWrite(GPIO_PORTB_BASE, LED_0, LED_0);
    	SysCtlDelay(SysCtlClockGet()/12);
    	UARTprintf("Rohit\n");
    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
    	SPIDataWrite(0xff);
    	UARTprintf("%x\n", SPIDataRead());
    	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1 , GPIO_PIN_1);
    	//SSIDataGet(SPI_BASE, &data);
    	//UARTprintf("%x\n", data);
    }
}

//*****************************************************************************
//
// NoritakeVFDInterface
/*
 * main.cpp
 *
 *  Created on: Jan 17, 2014
 *              Modified to work on the Stellaris Launchpad
 *              Gerard Sequeira
 *              43oh.com
 *              stellarisiti.com
 */

//*****************************************************************************

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "utils/uart1stdio.h"
#include "driverlib/pin_map.h"
#include "lowLevelDrivers\NoritakeY100\config.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "utils/uart1stdio.h"
#include "menu.h"
#include <string.h>

//*****************************************************************************
//
// Define pin to LED color mapping.
//
//*****************************************************************************

#define RED_LED   GPIO_PIN_1
#define BLUE_LED  GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

int
main(void)
{
	unsigned char test = 0;
    //
    // Setup the system clock to run at 50 Mhz from PLL with crystal reference
    //
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|
                    SYSCTL_OSC_MAIN);

    //
    // Enable and configure the GPIO port for the LED operation.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, RED_LED|BLUE_LED|GREEN_LED);

    // Noritake Busy and Reset pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput( RESET_PORT, RESET_PIN );
    GPIOPinTypeGPIOInput( BUSY_PORT, BUSY_PIN );
    GPIOPadConfigSet( BUSY_PORT, BUSY_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure( GPIO_PA0_U0RX );
    GPIOPinConfigure( GPIO_PA1_U0TX );
    GPIOPinTypeUART( GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 );
    UARTStdioInit( 0 );

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
    GPIOPinConfigure( GPIO_PC4_U1RX );
    GPIOPinConfigure( GPIO_PC5_U1TX );
    GPIOPinTypeUART( GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 );
    UART1StdioInitExpClk( 1, 38400 );


    SysTickPeriodSet(SysCtlClockGet() / 1000000 ); // 1us
    SysTickEnable();
    SysTickIntEnable();
    //test = 1;
    //UART1printf("Test begin %x", test);

    Menu_Init();

    Menu_Run();
}




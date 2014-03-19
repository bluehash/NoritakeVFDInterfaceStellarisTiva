#include <stddef.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/rom_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "config.h"
#include "driverlib/uart.h"
#include "utils/uart1stdio.h"
#include "../SysTickDriver/SysTickDriver.h"

#if NORITAKE_VFD_INTERFACE==0 && NORITAKE_VFD_SERIAL_SYNC==0


#include "interface.h"

void initPort()
{
	RAISE(RESET_PORT, RESET_PIN);
}


void writePort(uint8_t data)
{
	while (CHECK(BUSY_PORT, BUSY_PIN));
	UARTCharPut(UART1_BASE, data);
}

void enterDiagnosticMode()
{
}

void exitDiagnosticMode()
{

}

void readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize)
{
}

void hardReset() {
	initPort();
	_delay_ms(50);
	LOWER(RESET_PORT, RESET_PIN);
	_delay_ms(1);
	RAISE(RESET_PORT, RESET_PIN);
	_delay_ms(100);
	while (CHECK(BUSY_PORT, BUSY_PIN));
}

#endif

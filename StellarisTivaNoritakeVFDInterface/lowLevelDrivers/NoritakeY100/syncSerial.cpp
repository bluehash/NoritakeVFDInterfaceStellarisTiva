#include "config.h"

#if NORITAKE_VFD_INTERFACE==0 && NORITAKE_VFD_SERIAL_SYNC==1

#include <stddef.h>
#include <avr/io.h>
#include <util/delay.h>
#include "interface.h"

void initPort() {
	RAISE(OUT);
	RAISE(SCK); // enable pull-up before changing direction
	RAISE(RESET); // enable pull-up so no reset is triggered
	DIRECTION(RESET,1);
	DIRECTION(OUT,1);
	DIRECTION(SCK,1);
	DIRECTION(BUSY,0);
}

void writePort(uint8_t data) {
	while (CHECK(BUSY));
	for (int i=1; i <= 0x80; i<<=1) {
		LOWER(SCK);
		_delay_us(1);
		if (data & i) RAISE(OUT); else LOWER(OUT);
		RAISE(SCK);
		_delay_us(1);
	}
	_delay_us(17);
}

void enterDiagnosticMode() {
}

void exitDiagnosticMode() {
}

void readDiagnosticInfo(const uint8_t *cmd, size_t cmdSize, uint8_t *output, uint8_t outputSize) {
}

void hardReset() {
	initPort();
	LOWER(RESET);
	_delay_ms(1);
	RAISE(RESET);
	_delay_ms(100);
	while (CHECK(BUSY));
}

#endif

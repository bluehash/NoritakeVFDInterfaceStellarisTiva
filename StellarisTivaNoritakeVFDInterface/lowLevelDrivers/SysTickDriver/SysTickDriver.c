/*
 * SysTickDriver.c
 *
 *  Created on: Jan 17, 2014
 *      Author: gsequeira
 */
#include "SysTickDriver.h"

unsigned long ulgCounter = 0;
void SysTickCountDown( unsigned long lCountDown );

void SysTickCountDown( unsigned long lCountDown )
{
	ulgCounter = lCountDown;
}

void SysTickIntHandler(void)
{

	if ( ulgCounter == 0 )
		ulgCounter = 1;
	ulgCounter--;
}



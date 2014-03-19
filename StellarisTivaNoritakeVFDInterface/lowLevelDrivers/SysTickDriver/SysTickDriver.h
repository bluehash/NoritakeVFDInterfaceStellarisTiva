/*
 * SysTickDriver.h
 *
 *  Created on: Jan 17, 2014
 *      Author: gsequeira
 */

#ifndef SYSTICKDRIVER_H_
#define SYSTICKDRIVER_H_
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned long ulgCounter;
void SysTickIntHandler(void);
extern void SysTickCountDown( unsigned long lCountDown );
#ifdef __cplusplus
}
#endif
#endif /* SYSTICKDRIVER_H_ */

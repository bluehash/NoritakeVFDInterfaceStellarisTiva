#include "inc/hw_types.h"
#include "driverlib/sysctl.h"

// CPU frequency in Hertz
#define F_CPU                           50000000UL

#ifdef __AVR
    #include <util/delay.h>
    #include <avr/pgmspace.h>
#else
	//#include <unistd.h>
	#define _delay_us(x)            usleep(x)
	#define _delay_ms(x)            SysCtlDelay( x * 2500 );
    #define PROGMEM
    #define pgm_read_byte(p)        (*(uint8_t*)p)
#endif


//Delay time between the Atmel starting and the VFD module being
//initialized. This is necessary to allow the module's controller
//to initialize. This value will vary depend on the power supply
//and hardware setup. 500 ms is more than enough time for the
//module to start up. This delay will only occur the first time
//the CUY_init() or CUY_reset() method is called.
#define NORITAKE_VFD_RESET_DELAY        500

//Set to 1 to select the brightness correctly on models
//	  that have the 400% brightness levels. Set to 0
//	  for all other models.
//	  Modules that have the 400% brightness level have model
//      numbers in the following form:
//	    CU******-YX***
//        Example:
//            CU20027-YX1A
//            CU20027-YX100
#define NORITAKE_VFD_400_BRIGHTNESS     0

//The number of columns and lines on the display
#define NORITAKE_VFD_COLS               24
#define NORITAKE_VFD_LINES              4

// SELECT THE INTERFACE TO THE VFD MODULE
// NORITAKE_VFD_INTERFACE:
//  0 = Synchronous or Asynchronous Serial
//  1 = Parallel
//  2 = Linux serial device
#define NORITAKE_VFD_INTERFACE          0

// SELECT SYNCHRONOUS OR ASYNCHRONOUS FOR SERIAL
// NORITAKE_VFD_SERIAL_SYNC
//  0 = Asynchronous
//  1 = Synchronous
#define NORITAKE_VFD_SERIAL_SYNC        0


#if NORITAKE_VFD_INTERFACE==0
    //
    // SERIAL INTERFACE OPTIONS
    //
    //  NORITAKE_VFD_BAUD	    Baud rate
    //  OUT_PORT & OUT_PIN          Data line port & pin
    //  SCK_PORT & SCK_PIN          Clock line port & pin
    //  BUSY_PORT & BUSY_PIN        Busy line port & pin
    //  RESET_PORT & RESET_PIN      Reset line port & pin
    #if NORITAKE_VFD_SERIAL_SYNC==0
        //
        // ASYNCHRONOUS SERIAL INTERFACE OPTIONS
        //
        #define NORITAKE_VFD_BAUD       38400
        //#define OUT_PIN                 4
        //#define OUT_PORT                PORTG
        #define BUSY_PIN                GPIO_PIN_0
        #define BUSY_PORT               GPIO_PORTD_BASE
        #define RESET_PIN               GPIO_PIN_1
        #define RESET_PORT              GPIO_PORTD_BASE
    #else
        //
        // SYNCHRONOUS SERIAL INTERFACE OPTIONS
        //
        #define OUT_PIN                 4
        #define OUT_PORT                PORTG
        #define SCK_PIN                 3
        #define SCK_PORT                PORTG       
        #define BUSY_PIN                7
        #define BUSY_PORT               PORTB
        #define RESET_PIN               6
        #define RESET_PORT              PORTA
    #endif
#elif NORITAKE_VFD_INTERFACE==1
    //
    // I80 PARALLEL INTERFACE OPTIONS
    //  WR_PORT & WR_PIN        WR line port & pin
    //  BUSY_PORT & BUSY_PIN    Busy line port & pin
    //	D0_PORT & D0_PIN		Data bit 0 port & pin
    //	D1_PORT & D1_PIN		Data bit 1 port & pin
    //	D2_PORT & D2_PIN		Data bit 2 port & pin
    //	D3_PORT & D3_PIN		Data bit 3 port & pin
    //	D4_PORT & D4_PIN		Data bit 4 port & pin
    //	D5_PORT & D5_PIN		Data bit 5 port & pin
    //	D6_PORT & D6_PIN		Data bit 6 port & pin
    //	D7_PORT & D7_PIN		Data bit 7 port & pin
    //  Please refer to VFD module specification for pin assignments.
    #define BUSY_PIN                7
    #define BUSY_PORT               PORTA
    #define WR_PIN                  0
    #define WR_PORT                 PORTG
    #define RESET_PIN               2
    #define RESET_PORT              PORTG   
    #define D0_PORT                 PORTC
    #define D0_PIN                  0
    #define D1_PORT                 PORTC
    #define D1_PIN                  1
    #define D2_PORT                 PORTC
    #define D2_PIN                  2
    #define D3_PORT                 PORTC
    #define D3_PIN                  3
    #define D4_PORT                 PORTC
    #define D4_PIN                  4
    #define D5_PORT                 PORTC
    #define D5_PIN                  5
    #define D6_PORT                 PORTC
    #define D6_PIN                  6
    #define D7_PORT                 PORTC
    #define D7_PIN                  7    
#elif NORITAKE_VFD_INTERFACE==2
    #define NORITAKE_VFD_FILE       "/dev/ttyUSB0"
    #define NORITAKE_VFD_BAUD       38400
#endif

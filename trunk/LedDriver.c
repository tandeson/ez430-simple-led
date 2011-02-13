/*
 * File: LedDriver.c
 * Author: Thomas Anderson
 * 
 * Description: Driver for sending data out to a CAT4008 LED driver.
 * 
 * 
*/

#include "LedDriver.h"
#include "standard.h"

// Sets the data line based on the conditional (a). Toggles the clock
//  line to send out the new data.
#define CLOCK_OUT_BIT(a) {(a) ? \
	SET_BIT(SEND_BYTE_PORT,SEND_BYTE_DATA_BIT) : \
	CLEAR_BIT(SEND_BYTE_PORT,SEND_BYTE_DATA_BIT); \
	SET_BIT(SEND_BYTE_PORT,SEND_BYTE_CLOCK_BIT); \
	CLEAR_BIT(SEND_BYTE_PORT,SEND_BYTE_CLOCK_BIT);}
	
//-----------------------------------------------------------------
// sendByteSerial()
//-----------------------------------------------------------------
void sendByteSerial(unsigned char toSend)
{
	// Send out LED8 bit
	CLOCK_OUT_BIT(toSend & 0x80);
	
	// Send out LED7 bit
	CLOCK_OUT_BIT(toSend & 0x40);
		
	// Send out LED6 bit
	CLOCK_OUT_BIT(toSend & 0x20);
	
	// Send out LED5 bit
	CLOCK_OUT_BIT(toSend & 0x10);
	
	// Send out LED4 bit
	CLOCK_OUT_BIT(toSend & 0x08);
	
	// Send out LED3 bit
	CLOCK_OUT_BIT(toSend & 0x04);
	
	// Send out LED2 bit
	CLOCK_OUT_BIT(toSend & 0x02);
	
	// Send out LED1 bit
	CLOCK_OUT_BIT(toSend & 0x01);
	
	// Latch data into register
	SET_BIT(SEND_BYTE_PORT,SEND_BYTE_LATCH);
	CLEAR_BIT(SEND_BYTE_PORT,SEND_BYTE_LATCH);
	
	// Ensure that Clear is not set
    CLEAR_BIT(SEND_BYTE_PORT,SEND_BYTE_CLEAR);
}

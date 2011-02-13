#ifndef _LED_DRIVER_H
#define _LED_DRIVER_H
/*
 * File: LedDriver.h
 * Author: Thomas Anderson
 * 
 * Description: Driver for sending data out to a CAT4008 LED driver.
 * 
 * 
*/

//***************** Constants *****************
// On rising edge, data clocked in.
#define SEND_BYTE_CLOCK_BIT      (0x04)
#define SEND_BYTE_DATA_BIT       (0x02)
// On rising edge, data is latched.
#define SEND_BYTE_LATCH          (0x08)
// When high all LED clear, when low all LED set as known.
#define SEND_BYTE_CLEAR          (0x01)
#define SEND_BYTE_PORT          (P1OUT)

// ***************** Function Prototypes *****************.

//-----------------------------------------------------------------
// sendByteSerial()
//-----------------------------------------------------------------
// Clocks data into a CAT4008 LED driver. Assumes LED is left with
//  Clock line low. Data is sent in serially, High bit first. Data
//  is pulled in on the rising edge of the clock. Data is latched in
//  the driver when the latch bit goes from low->high.
void sendByteSerial(unsigned char toSend);

#endif // _LED_DRIVER_H

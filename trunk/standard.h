#ifndef _STANDARD_H_
#define _STANDARD_H_
/*
 * File: standard.h
 * Author: Thomas Anderson
 * 
 * Description: Shared project wide typedef, macros, etc
*/
#include <msp430f2012.h>

//***************** typedef ***************** 
typedef unsigned char boolean;

//***************** Defines *****************
#define TRUE    (1)
#define FALSE   (0)

#define MSEC_PER_SEC          (1000)
#define IN_KHZ(a)         (a * 1000)
#define IN_MHZ(a)         (a * 1000000)

// Set to match system clock 
#define DCO_CLOCK_HZ   (IN_MHZ(8))
#define VLP_CLOCK      (IN_KHZ(12))

// 16 Mhz
#define M_CLOCK_HZ  (DCO_CLOCK_HZ)

// 12 KHz
#define A_CLOCK_HZ  (VLP_CLOCK)

// 16 MHz
#define SM_CLOCK_HZ (DCO_CLOCK_HZ / 8)

// about 12 Khz
#define TIMER_A_CLOCK_HZ (SM_CLOCK_HZ / 8)

// Setup References for Interrupt time.
#define TIMER_A_INT_TIME_MSEC (1)
#define TIMER_A_INT_COUNT    (TIMER_A_CLOCK_HZ / (MSEC_PER_SEC/TIMER_A_INT_TIME_MSEC))

#define NULL (0)
//***************** Macro *****************
#define SET_BIT(port,bit)   (port |= bit)
#define CLEAR_BIT(port,bit) (port &= (0xFF^bit))



#endif // _STANDARD_H_

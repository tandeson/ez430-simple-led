/*
    Test of LED
    
    Code Composer test
*/
#include "standard.h"
#include "LedDriver.h"
#include "LedPattern.h"
#include "buttons.h"

// **************** Function Prototypes *****************.
void timerAInterrupt(void);
void setupHardware(void);

//***************** Functions ***************************
int main(void)
{	
	// Stop WDT
    WDTCTL = WDTPW + WDTHOLD;                 
	
	// Configure hardware registers. (e.g. Port1, Clock, etc)
	setupHardware();
	
	// Target interrup time.
	CCR0 = TIMER_A_INT_COUNT;
	
    // Enable interrupts
     _BIS_SR(GIE);
     
	// Loop forever
	for(;;)
	{
		// Only show 2 LED at any one time, cycle though
		//  as fast as possible. This limits power consumption
		//  But still shows patterens to a human as expected.
		displayLedSeq();
		
		LPM0;
	}
}

// ********************************************
// Timer_A Interrupt
// ********************************************
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A(void)
{	
	// **********************
	// Handle Button State
	// **********************
	updateButtons();
	
	// **********************
	// Manage LED
	// **********************
	
	// Look for button press, and on a new press, take action
    if (getButtonOne() == BUTTON_PRESSED)
    {
    	incrimentSeq();
    	
    	if(TRUE == isAllLedOff())
    	{
    		sendByteSerial(0x00);
    	}
    }
    else if (getButtonTwo() == BUTTON_PRESSED)
    {
    	//decramenttSeq();
    	nextSpeed();
    }
	
	// Update Array pointer / Array Index
	updateLedSeq();
	
	if(TRUE != isAllLedOff())
	{ 
	    // ensure we run at least once after the interrupt
	    LPM0_EXIT;
	}

	// Setup for next interrupt
	CCR0 += TIMER_A_INT_COUNT;
}

// ********************************************
// Setup Input / Output pins , clocks, etc
// ********************************************
void setupHardware(void)
{
	// ******************
	// * Clock
    // ******************
	
	// Setup the Clock for 8 MHz (see datasheet)
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL  = CALDCO_8MHZ;
    
    // Divide SMCLK by 8, use DCO for SMCLK, Internal Resistor
    BCSCTL2 = (SELM_0 + DIVM_0 + DIVS_3);
    	
	// ******************
	// * Port 1 Setup
	// ******************
	
	// Set direction on Port 1
	SET_BIT(P1DIR,
	    (SEND_BYTE_CLOCK_BIT 
	    + SEND_BYTE_DATA_BIT 
	    + SEND_BYTE_LATCH 
	    + SEND_BYTE_CLEAR)
	);
	
	// Ensure Switches are set as inputs
	CLEAR_BIT(BUTTONS_PORT_DIRECTION,
	    (SWITCH_ONE 
	    + SWITCH_TWO)
	);
	
	// ******************
	// * Setup Led Driver state
	// ******************
	
	// Set initial state for LED Driver
	CLEAR_BIT(SEND_BYTE_PORT,SEND_BYTE_CLOCK_BIT);
	sendByteSerial(0x00);
	
	// ******************
	// * Timer A setup
	// ******************
	
	// Input SMCLK, Div by 8, continouse mode.
	TACTL = (TASSEL_2 + ID_3 + MC_2);
	
	// Timer A CCR0 interrupt enabled                            
	CCTL0 = CCIE;  
}

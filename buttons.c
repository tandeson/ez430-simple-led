/*
 * File: buttons.c
 * Author: Thomas Anderson
 * 
 * Description: Driver for input buttons.
 * 
 * 
*/
#include "standard.h"
#include "buttons.h"

// Set Debounce count based on TimerA
#define MSEC_OF_DEBOUNCE     (200)
#define BUTTON_STATE_SWITCH  (MSEC_OF_DEBOUNCE / TIMER_A_INT_TIME_MSEC)

// Structure for keeping track of buttons
typedef struct 
{
	unsigned char counter;  // count for button debouncing
	boolean stateRead;      // Has state been read since last change
	enum BUTTON_STATE state;// State (pressed or released)
}buttonState;

// Private Funciton headers
enum BUTTON_STATE readButton(buttonState* toRead);
void processButton(buttonState* toUpdate, boolean inputState);

// File Global Variables
static buttonState one = {0,TRUE,BUTTON_RELEASE};
static buttonState two = {0,TRUE,BUTTON_RELEASE};

// ** Functions ********************************************

// ******************************
// * Read Functions for buttons *
// ******************************
enum BUTTON_STATE getButtonOne() { return readButton(&one); }
enum BUTTON_STATE getButtonTwo() { return readButton(&two); }

// ***********************************************
// * Process raw input state to get button state *
// ***********************************************	
void updateButtons()
{	
	// Get current switch state.
	boolean switchOne = (BUTTONS_PORT_IN & SWITCH_ONE);
	boolean switchTwo = (BUTTONS_PORT_IN & SWITCH_TWO);
	
	processButton(&one,switchOne);
	processButton(&two,switchTwo);
}

// ****************************
// * Private Helper Functions *
// ****************************

// Run the button debounce logic on one of our buttons.
void processButton(buttonState* toUpdate, boolean inputState)
{
	if(toUpdate->state == BUTTON_PRESSED)
	{
		// If signal is present, incriment counter
		if (inputState)
		{ 
			toUpdate->counter++;
		}
		else
		{
			// if this is greater then 0, move back to 0
			if (toUpdate->counter > 0 ) toUpdate->counter--;
		}
		
		if (toUpdate->counter >= BUTTON_STATE_SWITCH )
		{
			toUpdate->counter = 0;
			toUpdate->stateRead = FALSE;
			toUpdate->state = BUTTON_RELEASE;
		}
	}
	else if (toUpdate->state == BUTTON_RELEASE)
	{
		// If signal is present, incriment counter
		if (!inputState)
		{ 
			toUpdate->counter++;
		}
		else
		{
			// if this is greater then 0, move back to 0
			if (toUpdate->counter > 0 ) toUpdate->counter--;
		}
		
		if (toUpdate->counter >= BUTTON_STATE_SWITCH )
		{
			toUpdate->counter = 0;
			toUpdate->stateRead = FALSE;
			toUpdate->state = BUTTON_PRESSED;
		}
	}
}

// Handles reading a button
enum BUTTON_STATE readButton(buttonState* toRead)
{
	if (toRead->stateRead == FALSE)
	{
	    toRead->stateRead = TRUE;
	    return toRead->state;
	}
	else
	{
		return BUTTON_NONE;
	}
}

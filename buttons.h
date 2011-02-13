#ifndef _BUTTONS_H
#define _BUTTONS_H
/*
 * File: LedPattern.h
 * Author: Thomas Anderson
 * 
 * Description: Header for managing input buttons.
 * 
 * This code assumes that: 
 *   - all buttons are on one port.
 *   - Buttons have an external pull up to VCC.
 *   - Buttons will be read one time only. (see getButton___ functions)
 *   - Button debouncing will be done by regually calling updateButtons
 *   - Button debouncing will be done by timer A (see butons.c file).
*/

// enums
// Enumeration state 
enum BUTTON_STATE
{
	BUTTON_NONE, // Unknown state.
	BUTTON_PRESSED, // The button is pressed.
	BUTTON_RELEASE // The button has been released
};

// Defines
#define BUTTONS_PORT_DIRECTION (P1DIR)
#define BUTTONS_PORT_IN        (P1IN)

#define SWITCH_ONE      (0x10) 
#define SWITCH_TWO      (0x20)

// Functions
enum BUTTON_STATE getButtonOne();
enum BUTTON_STATE getButtonTwo();
void updateButtons();

#endif // _BUTTONS_H

/*
 * File: LedPattern.c
 * Author: Thomas Anderson
 * 
 * Description: Sequences for a 8 LED blink patterns.
 * 
 * 
*/
#include "standard.h"
#include "LedPattern.h"
#include "LedDriver.h"

#define WAIT_IN_MSEC(a)  ((a/TIMER_A_INT_TIME_MSEC)+1 )

// ***************************************
// Function Prototypes
// ***************************************
unsigned char countUp(unsigned char* lastCount);

// ***************************************
// Constant Data
// ***************************************

// Leds blink between even and odd
static const unsigned char SideToSide[2] =
{
	0xAA, // 1010 1010
	0x55  // 0101 0101
};

// ***** Raw Patterns ****
static unsigned char FollowFillPattern[14] =
{
	0x80, // 1000 0000
	0xC0, // 1100 0000
	0xE0, // 1110 0000
	0xF0, // 1111 0000
	0xF8, // 1111 1000
	0xFC, // 1111 1100
	0xFE, // 1111 1110
	0xFF, // 1111 1111
	0xFE, // 1111 1110
	0xFC, // 1111 1100
	0xF8, // 1111 1000
	0xF0, // 1111 0000
	0xE0, // 1110 0000
	0xC0, // 1100 0000
};

static unsigned char WalkOnBy[16] =
{
	0x80, // 1000 0000
	0xC0, // 1100 0000
	0xE0, // 1110 0000
	0xF0, // 1111 0000
	0xF8, // 1111 1000
	0xFC, // 1111 1100
	0xFE, // 1111 1110
	0xFF, // 1111 1111
	0x7F, // 0111 1111
	0x3F, // 0011 1111
	0x1F, // 0001 1111
	0x0F, // 0000 1111
	0x07, // 0000 0111
	0x03, // 0000 0011
	0x01, // 0000 0001
	0x00, // 0000 0000
};

static const unsigned char UpDown[14] =
{
	0x80, // 1000 0000
	0x40, // 0100 0000
	0x20, // 0010 0000
	0x10, // 0001 0000
	0x08, // 0000 1000
	0x04, // 0000 0100
	0x02, // 0000 0010
	0x01, // 0000 0001
	0x02, // 0000 0010
	0x04, // 0000 0100
	0x08, // 0000 1000
	0x10, // 0001 0000
	0x20, // 0010 0000
	0x40, // 0100 0000
};

static const unsigned char doubleSpin[6] =
{
	0x18, // 0001 1000
	0x24, // 0010 0100
	0x42, // 0100 0010
	0x81, // 1000 0001
	0x42, // 0100 0010
	0x24, // 0010 0100
};

static const unsigned char allOn[1] =
{
	0xFF, // 1111 1111
};

static const unsigned char allOff[1] =
{
	0x00, // 0000 0000
};

static const unsigned char Scatter[19] =
{
	0x10, // 0001 0000
	0x01, // 0000 0001
	0x00, // 0000 0000
	0x40, // 0100 0000
	0x00, // 0000 0000
	0x08, // 0000 1000
	0x1C, // 0001 1100
	0x08, // 0000 1000
	0x00, // 0000 0000
	0x00, // 0000 0000
	0x02, // 0000 0010
	0x20, // 0010 0000
	0x80, // 1000 0000
	0x00, // 0000 0000
	0x23, // 0010 0011
	0x08, // 0000 1000
	0x20, // 0010 0000
	0x50, // 0101 0000
	0x08, // 0000 1000
};

//-----------------------------------------------------------------------------

// **** Sequences ****
static const ledSequence SideToSideSeq = { 
	SideToSide,
	2,
	DATA_TYPE_ARRAY
};

static const ledSequence FollowSeq = { 
	UpDown,
	8,
	DATA_TYPE_ARRAY,
	NULL
};

static const ledSequence UpDownSeq = {
	 UpDown,
	 14,
	 DATA_TYPE_ARRAY,
	 NULL
};

static const ledSequence FollowFillPatternSeq = {
	FollowFillPattern,
	8,
	DATA_TYPE_ARRAY,
	NULL
};

static const ledSequence FollowFillUpDownSeq  = {
    FollowFillPattern,
    14,
    DATA_TYPE_ARRAY,
    NULL
};

static const ledSequence DoubleSpinSeq = {
    doubleSpin,
    6,
    DATA_TYPE_ARRAY,
    NULL
};

static const ledSequence DoubleOutSeq = {
	doubleSpin,
	4,
	DATA_TYPE_ARRAY,
	NULL
};

static const ledSequence AllOnSeq = {
	allOn,
	1,
	DATA_TYPE_ARRAY,
	NULL
};

static const ledSequence AllOffSeq = {
	allOff,
	1,
	DATA_TYPE_ARRAY,
	NULL
};

static const ledSequence ScatterSeq = {
    Scatter,
    19,
    DATA_TYPE_ARRAY,
    NULL
};

static const ledSequence WalkOnBySeq = {
    WalkOnBy,
    16,
    DATA_TYPE_ARRAY,
    NULL
};    

static const ledSequence countSeq = {
	NULL,
	1,
	DATA_TYPE_FUNCTION,
	&countUp
};

//-----------------------------------------------------------------------------
    
// Table of all sequences
const ledSequence* const AllSequences[NUM_OF_SEQUENCES] =
{
	&FollowSeq,
	&SideToSideSeq,
	&UpDownSeq,
	&FollowFillPatternSeq,
	&FollowFillUpDownSeq,
	&DoubleSpinSeq,
	&DoubleOutSeq,
	&AllOnSeq,
	&WalkOnBySeq,	
	&ScatterSeq,
	&countSeq,
	&AllOffSeq, // Must be last
};

// Table of all speeds
const unsigned char AllSpeeds[NUM_OF_SPEEDS] =
{
	WAIT_IN_MSEC(30),
	WAIT_IN_MSEC(50),
	WAIT_IN_MSEC(70),
	WAIT_IN_MSEC(90),
	WAIT_IN_MSEC(110),
	WAIT_IN_MSEC(200)
};

//-----------------------------------------------------------------------------

// File Static Variables
static unsigned char arrayIndex = 0;
static unsigned char countDownTimer = 0;
static unsigned char seqIndex = 0;
static unsigned char countTimeIndex = 3;
static unsigned char functionReturn = 0;

// Move up one LED Sequence.
void incrimentSeq()
{
	seqIndex++;
	if (seqIndex >= NUM_OF_SEQUENCES) seqIndex = 0;
	
	countDownTimer = 0;
	arrayIndex = 0;
	functionReturn = 0;
	
	sendByteSerial(AllSequences[seqIndex]->patternArray[arrayIndex]); 
}

// Move down one LED Sequence.
void decramenttSeq()
{
	seqIndex--;
	if (seqIndex >= NUM_OF_SEQUENCES) seqIndex = (NUM_OF_SEQUENCES - 1);
	
	countDownTimer = 0;
	arrayIndex = 0;
	functionReturn = 0;
	
	displayLedSeq(); 
}

// Itterate though our selection of speeds.
void nextSpeed()
{
	if (countTimeIndex >= (NUM_OF_SPEEDS - 1)) 
	{
		countTimeIndex = 0;
	}
	else 
	{
		countTimeIndex++;
	}
}

// Use our state variable to update the LED	
void updateLedSeq()
{
	if(countDownTimer < AllSpeeds[countTimeIndex])
	{
		// Not done counting yet, count up
		countDownTimer++;
	}
	else
	{
	    if(AllSequences[seqIndex]->type == DATA_TYPE_ARRAY)
	    {
			// update the array
			arrayIndex++;
			
			// Ensure we don't over index this array
			if(arrayIndex >= AllSequences[seqIndex]->numPatternSteps)
			{
				arrayIndex = 0;
			}
	    }
	    else if (AllSequences[seqIndex]->type == DATA_TYPE_FUNCTION)
	    {
	    	functionReturn = (*AllSequences[seqIndex]->pPatternFunction)(&arrayIndex);
	    }
	    
		// Setup the count down timer
		countDownTimer = 0;
	}
}

/*
    Patterns 
    0000 0011
    0011 1000
    1100 0000
*/  
void displayLedSeq()
{
	static unsigned char bitMask = 0x01;
	unsigned char toSendData = 0;
	
	// Set the LED pattern
	_DINT();
	
	if(AllSequences[seqIndex]->type == DATA_TYPE_ARRAY)
	{
		toSendData = AllSequences[seqIndex]->patternArray[arrayIndex];
	}
	else
	{
		toSendData = functionReturn;
	}
	
	sendByteSerial((toSendData & bitMask));
		
	// Shift up one bit, if we shift out, reset to bit 0
	bitMask <<= 1;
	if (bitMask == 0) bitMask = 0x01;
	
	_EINT();
}

// Lets caller know if this is the "off" sequence. 
unsigned char isAllLedOff()
{
	if ((NUM_OF_SEQUENCES - 1) == seqIndex)
	    return TRUE;
	else
	    return FALSE;
}

// Make the LED a binary counter
unsigned char countUp(unsigned char* lastCount)
{
	*lastCount = *lastCount + 1;
	return *lastCount;
}


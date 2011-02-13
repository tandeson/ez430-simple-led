#ifndef _LED_PATTERN_H
#define _LED_PATTERN_H
/*
 * File: LedPattern.h
 * Author: Thomas Anderson
 * 
 * Description: Header for sharing LED blink patterns
 * 
*/
enum DATA_TYPE
{
	DATA_TYPE_ARRAY,
	DATA_TYPE_FUNCTION
};
// Structures
typedef struct 
{
	const unsigned char* const patternArray; // array of bytes
	unsigned char numPatternSteps; // number of steps in the array
	enum DATA_TYPE type;
	unsigned char (*pPatternFunction)(unsigned char*);
} ledSequence;

// Number of sequences
#define NUM_OF_SEQUENCES  (12)
#define NUM_OF_SPEEDS  (6)

// Share The table
extern const ledSequence* const AllSequences[NUM_OF_SEQUENCES];

// Functions
void incrimentSeq();
void decramenttSeq();

void nextSpeed();

// Change which sequence data is pointed to.
void updateLedSeq();

// Output sequience data.
void displayLedSeq();


// check to see if we are at "off" sequence
unsigned char isAllLedOff();
#endif // _LED_PATTERN_H

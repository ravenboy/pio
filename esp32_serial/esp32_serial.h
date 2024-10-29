

#ifndef _ESP32_SERIAL_H
#define _ESP32_SERIAL_H
#include "SdFat.h"
#include "sdios.h"

/***
 * 
 *  serial I/O Methods for ESP32 
 * these are overloading functions that overload functions of the same name in t he cout_liabrary because of
 * for for ESP32, the Serial class is implemented as a USBCDC object, not a HardwareSerial,
 * because its used for flashing the board, and for serial communication)
 * 
 */

// clearSerInput: clears the serial input buffer
extern void clearSerInput(HWCDC& ser);

// DumpSer: dumps the serial input buffer
extern void DumpSer(HWCDC& ser);

// GetSerChar: gets a single character from the serial input buffer
extern char GetSerChar(HWCDC& ser);

// GetSerString: gets a string of characters from the serial input buffer
extern void GetSerString(HWCDC& ser, char *chars,int numChars, const __FlashStringHelper* prompt = NULL);

// GetOption: gets a single character from the serial input buffer. 
// Validates that the character is one of the options, else prompts the user to enter a valid character
extern char GetOption(HWCDC& ser, char options[2], const __FlashStringHelper* prompt = NULL);

// clearSerInput: clears the serial input buffer
extern void clearSerInput(USBCDC& ser);

// DumpSer: dumps the serial input buffer
extern void DumpSer(USBCDC& ser);

// GetSerChar: gets a single character from the serial input buffer
extern char GetSerChar(USBCDC& ser);

// GetSerString: gets a string of characters from the serial input buffer
extern void GetSerString(USBCDC& ser, char *chars,int numChars, const __FlashStringHelper* prompt = NULL);

// GetOption: gets a single character from the serial input buffer. 
// Validates that the character is one of the options, else prompts the user to enter a valid character
extern char GetOption(USBCDC& ser, char options[2], const __FlashStringHelper* prompt = NULL);


#endif //_ESP32_SERIAL_H


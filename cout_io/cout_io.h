

#ifndef _COUT_IO_H
#define _COUT_IO_H

#include "SdFat.h"
#include "sdios.h"

class MyOutStream: public ArduinoOutStream{
  /***
   * 
   * class MyOutStream: this class is used to override the putstr method in ArduinoOutStream
   * so that it can properly handle __FlashStringHelper* strings when used in conjunction
   * with the << operator (for ESP e32 chips, can also be used with other boards)
   */

private:

  
public:
 
  MyOutStream(Stream& s):ArduinoOutStream(s){}
  explicit MyOutStream(print_t& pr) : ArduinoOutStream(pr) {}

  void putstr(__FlashStringHelper* str);
};

// serial input (from user) Methods - 

// clearSerInput: clears the serial input buffer
extern void clearSerInput(HardwareSerial& ser);

// DumpSer: dumps the serial input buffer
extern void DumpSer(HardwareSerial& ser);

// GetSerChar: gets a single character from the serial input buffer
extern char GetSerChar(HardwareSerial& ser);

// GetSerString: gets a string of characters from the serial input buffer
extern void GetSerString(HardwareSerial& ser, char *chars,int numChars, const __FlashStringHelper* prompt = NULL);

// GetOption: gets a single character from the serial input buffer. 
// Validates that the character is one of the options, else prompts the user to enter a valid character
extern char GetOption(HardwareSerial& ser, char options[2], const __FlashStringHelper* prompt = NULL);

#endif //_COUT_IO_H


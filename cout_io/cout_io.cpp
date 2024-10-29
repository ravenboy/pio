

#include <wstring.h>
#include <Arduino.h>

#include "cout_io.h"
void MyOutStream::putstr(__FlashStringHelper* str) { 

  char buffer[strlen_P((PGM_P)str)+1];
  strcpy_P(buffer,(PGM_P)str);
  ArduinoOutStream::putstr(buffer);
  }

void clearSerInput(HardwareSerial& ser) {
  /***
   * clearSerInput: gets called if board is an ESP32, and ser is Serial1 or 2
   * or if board is other than ESP32
   * 
   */
  uint32_t m = micros();
  do {
      if(ser.read() >= 0) {
      m = micros();
      }
  } while (micros() - m < 10000);
}

void DumpSer(HardwareSerial& ser){
  /***
   * DumpSer: gets called if board is an ESP32, and ser is Serial1 or 2
   * or if board is other than ESP32
   * 
   */
  do {
        delay(10);
      } while (ser.available() && ser.read() >= 0);
}

char GetSerChar(HardwareSerial& ser){
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial1 or 2
   * or if board is other than ESP32
   * 
   */
  while (!ser.available()) {
        yield();
      }

  return ser.read();
}

void GetSerString(HardwareSerial& ser, char *chars,int numChars, const __FlashStringHelper* prompt) {
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial1 or 2
   * or if board is other than ESP32
   * 
   */

  byte ndx = 0;

  if(prompt != NULL){
    ser.print(prompt);
  }

  while(ndx < numChars-1){ //leave room for null terminator
    if (ser.available()) {
        chars[ndx] = ser.read();
        
        if(chars[ndx]=='\n'){
          chars[ndx-1] = '\0';
          break;
        }
        ser.print(chars[ndx++]);
    }
  }
  ser.print(F("\nYou entered: "));
  ser.print(chars);
  ser.print(F("\n"));
}

char GetOption(HardwareSerial& ser, char options[], const __FlashStringHelper* prompt){
  /***
   * GetOption: gets called if board is an ESP32, and ser is Serial1 or 2
   * or if board is other than ESP32
   * 
   */

  char c = 0;

  clearSerInput(ser);
  if(prompt){
    ser.print(prompt);
  }

  while(true){
    while (!ser.available()) { //wait for user to enter a character
      yield();
      }

    c = ser.read();
    ser.print(c);
    if(options){

      for(int i = 0; i < sizeof(options)-1; i++){
        if(c == options[i]){
          ser.print(F("\nprintf: you entered: "));
          ser.print(c);
          ser.print(F("\"\n"));
          return c;
        }
      }
      ser.print(F("\n\"%c\""));
      ser.print(c);
      ser.print(F("\" is an invalid option, try again: "));

    }else{
      ser.print(F("\nYou entered: "));
      ser.print(c);
      ser.print(F("\n"));

      return c;
    }
  }
}

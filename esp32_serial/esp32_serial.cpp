

#include <wstring.h>

#include "esp32_serial.h"
  

void clearSerInput(HWCDC& ser) {
  /***
   * clearSerInput: gets called if board is an ESP32, and ser is Serial
   * 
   */
  uint32_t m = micros();
  do {
      if(ser.read() >= 0) {
      m = micros();
      }
  } while (micros() - m < 10000);
}

void DumpSer(HWCDC& ser){
  /***
   * DumpSer: gets called if board is an ESP32, and ser is Serial
   * 
   */
  do {
        delay(10);
      } while (ser.available() && ser.read() >= 0);
}

char GetSerChar(HWCDC& ser){
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial
   * 
   */
  while (!ser.available()) { //wait for user to enter a character
        yield();
      }

  return ser.read();
}

void GetSerString(HWCDC& ser, char *chars,int numChars, const __FlashStringHelper* prompt) {
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial 
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

  ser.printf("\nYou entered: \"%s\"", chars);
}

char GetOption(HWCDC& ser, char options[], const __FlashStringHelper* prompt){ 
  /******
   * GetOption: gets called if board is an ESP32, and ser is Serial
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
          ser.printf("\nYou entered: \"%c\"\n", c);

          return c;
        }
      }
      ser.printf("\n\n\"%c\" is an invalid option, try again: ", c);

    }else{
      ser.printf("\nYou entered: \"%c\"\n", c);
      return c;
    }
  }
}


void clearSerInput(USBCDC& ser) {
  /***
   * clearSerInput: gets called if board is an ESP32, and ser is Serial
   * 
   */
  uint32_t m = micros();
  do {
      if(ser.read() >= 0) {
      m = micros();
      }
  } while (micros() - m < 10000);
}

void DumpSer(USBCDC& ser){
  /***
   * DumpSer: gets called if board is an ESP32, and ser is Serial
   * 
   */
  do {
        delay(10);
      } while (ser.available() && ser.read() >= 0);
}

char GetSerChar(USBCDC& ser){
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial
   * 
   */
  while (!ser.available()) { //wait for user to enter a character
        yield();
      }

  return ser.read();
}

void GetSerString(USBCDC& ser, char *chars,int numChars, const __FlashStringHelper* prompt) {
  /***
   * GetSerString: gets called if board is an ESP32, and ser is Serial 
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

  ser.printf("\nYou entered: \"%s\"", chars);
}

char GetOption(USBCDC& ser, char options[], const __FlashStringHelper* prompt){ 
  /******
   * GetOption: gets called if board is an ESP32, and ser is Serial
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
          ser.printf("\nYou entered: \"%c\"\n", c);

          return c;
        }
      }
      ser.printf("\n\n\"%c\" is an invalid option, try again: ", c);

    }else{
      ser.printf("\nYou entered: \"%c\"\n", c);
      return c;
    }
  }
}
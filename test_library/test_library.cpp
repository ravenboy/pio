/*************************************************
*
* sendbrandandtests.cpp
# header for Shared Library for Boardwalk test scripts
* Copyright 2022 John Pence, Pence Enterprises
*
*************************************************/

#ifdef FAST_TESTS
#include "am2320.h"
#include "ds18b20.h"
#include "shared.h"
#include "do_clock.h"
#endif

void SendBrandAndTests(void){
#ifdef FAST_TESTS
  cout << F("BEGIN Fast Tests\n"); //dont change this string, bwker knows it
 
  if(Print_DateTime()){ //if RTC read fails, then Print_DateTime returns false
      Fast_DS18B20_Test(1);
      Fast_AM2320_Test(1);
      // dont need  -  and has a bug that corrupts sd card Fast_SD_Test();
      SendSDFatState();
      ListFiles();
      SendBrand();
  }
  cout << F("END Fast Tests\n"); //dont change this string, bwker knows it
  
#endif
}
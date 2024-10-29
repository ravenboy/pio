
#include <Wire.h>
#include "rtc_library.h"
#include "cout_io.h"

#ifdef ESP_32
#include "esp32_serial.h"
#endif

#include "sd_library.h"

extern MyOutStream cout;

#ifndef DS3231_I2C_ADDRESS
#define DS3231_I2C_ADDRESS 0x68
#endif

typedef char* PPM;

void GetSettings(char * settings){
    //example respomse:
    //6 is saturday
    //07/01/22 09:12:00 6x00
    //its  null terminated by sender
    //clearSerInput(Serial);
    DumpSer(Serial);
     
    cout << F("\nEnter clock settings: ") << endl;

   int idx=0;
    while(true){
        if (Serial.available() > 0) {
          settings[idx] = Serial.read();
          //char c =  Serial.read();
          //cout << settings[idx];
            
#ifdef DEBUG_GetSettings
            
            cout << F("received: ") << settings[idx] << endl;
#endif
            if (settings[idx++] == '\0'){
                break;
            }
        }
    }
    cout << F("\nYou Entered: ") << settings << endl;
}


class cDS3231 {
private:
     
  byte decToBcd(byte val)
  {
      return( (val/10*16) + (val%10) );
  }
  
  // Convert binary coded decimal to normal decimal numbers
  byte bcdToDec(byte val)
  {
      return( (val/16*10) + (val%16) );
  }
protected:
    
public:
  cDS3231(void);
  virtual ~cDS3231(void);
  
  
  void SetTime(byte second,
                byte minute,
                byte hour,
                byte dayOfWeek, //monday is 1. sunday is 7
                byte dayOfMonth,
                byte month,
                byte year);
  
  bool ReadTime(byte *second,
                byte *minute,
                byte *hour,
                byte *dayOfWeek,
                byte *dayOfMonth,
                byte *month,
                byte *year);
  
  bool PrintTime(void);
  
  void FormatTimeS(  char* pstrDateTime_OUT,
                    const byte second,
                    const byte minute,
                    const byte hour,
                    const byte dayOfWeek,
                    const byte dayOfMonth,
                    const byte month,
                    const byte year,
                    const bool bShort = false);
};

cDS3231::cDS3231(void){
    //Wire.begin(); // all scripts run the fast tests, WireBegin is in Start_AM2320
    Serial.println(F("Start DS3231"));
    
}

cDS3231::~cDS3231(void){
    
}

void cDS3231::SetTime(byte second,
                      byte minute,
                      byte hour,
                      byte dayOfWeek,
                      byte dayOfMonth,
                      byte month,
                      byte year){

    // sets time and date data to DS3231
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set next input to start at the seconds register
    Wire.write(decToBcd(second)); // set seconds
    Wire.write(decToBcd(minute)); // set minutes
    Wire.write(decToBcd(hour)); // set hours
    Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
    Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
    Wire.write(decToBcd(month)); // set month
    Wire.write(decToBcd(year)); // set year (0 to 99)
    Wire.endTransmission();
}

// callers of this member need to check year, is 1 retry Or punt
bool cDS3231::ReadTime(byte *second,
                       byte *minute,
                       byte *hour,
                       byte *dayOfWeek,
                       byte *dayOfMonth,
                       byte *month,
                       byte *year){
   
    bool bResult = true;
    Wire.beginTransmission(DS3231_I2C_ADDRESS);
    Wire.write(0); // set DS3231 register pointer to 00h
    Wire.endTransmission();
    uint8_t reqResult = Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
    if(reqResult){
        // request seven bytes of data from DS3231 starting from register 00h
        *second = bcdToDec(Wire.read() & 0x7f);
        *minute = bcdToDec(Wire.read());
        *hour = bcdToDec(Wire.read() & 0x3f);
        *dayOfWeek = bcdToDec(Wire.read());
        *dayOfMonth = bcdToDec(Wire.read());
        *month = bcdToDec(Wire.read());
        *year = bcdToDec(Wire.read());
 #ifdef DEBUG_cDS3231_ReadTime
        Serial.print(F("second: "));
        Serial.print(*second);

        Serial.print(F(" minute: "));
        Serial.print(*minute);

        Serial.print(F(" hour: "));
        Serial.println(*hour);

        Serial.print(F(" dayOfWeek: "));
        Serial.print(*dayOfWeek );

        Serial.print(F(" dayOfMonth: "));
        Serial.print(*dayOfMonth );

   
        Serial.print(F(" month: "));
        Serial.print(*month);

        Serial.print(F(" year: "));
        Serial.println(*year);

 #endif      
        if(! (((*second >= 0) && (*second <= 60)) &&
            ((*minute >= 0) && (*minute <= 60)) &&
            ((*hour >= 0) && (*hour <= 23)) &&
            ((*dayOfMonth >= 1) && (*dayOfMonth <= 31)) &&
            ((*dayOfWeek >= 1) && (*dayOfWeek <= 7)) &&
            ((*month >= 1) && (*month <= 12)))){
                // return values that will work but not crash
                // when converted
                //callers of this member need to check, if year is 1 try again
                // or punt
                Serial.println(F("RTC Read succeeded but data is bad"));
                /*
                *second = 1;
                *minute = 1;
                *hour = 1;
                *dayOfWeek = 1;
                *dayOfMonth = 1;
                *month = 1;
                *year = 1; // callers of this member need to check year, is 1 retry Or punt
               */
                
                //no let caller -> ERROR(eIndERR_RTC_BAD_TIME_FETCHED);
                bResult = true;

            }else{
            
            cout << F("RTC Read succeeded and data is good") << endl;
            
            bResult = true;
            }

    }else{
        //Serial.println(F("DS3221 RTC Failure"));
        bResult = false;
    }
    return bResult;
}
//////////////////////
 void cDS3231::FormatTimeS(char* pstrDateTime_OUT,
                          const byte second,
                          const byte minute,
                          const byte hour,
                          const byte dayOfWeek,
                          const byte dayOfMonth,
                          const byte month,
                          const byte year,
                          const bool bShort )
{
 
    int iIndex = 0;
    char pColon[2];
    char pSlash[2];
    char pSpace[2];

    strcpy_P(pColon,(PGM_P)F(":"));
    strcpy_P(pSpace,(PGM_P)F(" "));
    strcpy_P(pSlash,(PGM_P)F("/"));
 
    for(int i = 0 ;i<64;i++){
        pstrDateTime_OUT[i]='\0';
    }

    for(int i = 0 ;i<64;i++){
        pstrDateTime_OUT[i]='\0';
    }

    if (hour<10)
    {
        pstrDateTime_OUT[iIndex++]='0';
    }

    char* p = itoa(hour,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);
    pstrDateTime_OUT[iIndex++]= pColon[0];

    if (minute<10)
    {
        pstrDateTime_OUT[iIndex++]='0';
    }

    p = itoa(minute,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);
    pstrDateTime_OUT[iIndex++]= pColon[0];

    if (second<10){
        pstrDateTime_OUT[iIndex++]='0';
    }

    p = itoa(second,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);
    pstrDateTime_OUT[iIndex++]= pSpace[0];
    //this is in USA format mm/dd/year
    p = itoa(month,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);
    pstrDateTime_OUT[iIndex++]= pSlash[0];

    p = itoa(dayOfMonth,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);
    pstrDateTime_OUT[iIndex++]= pSlash[0];

    p = itoa(year,&pstrDateTime_OUT[iIndex],10);
    iIndex+=strlen(p);

    if(!bShort){
        if((dayOfWeek <= 8) | (dayOfWeek >= 1)){
            const char dayStr0[] PROGMEM = " Err";
            
            const char dayStr1[] PROGMEM = " Monday";
            const char dayStr2[] PROGMEM = " Tuesday";
            const char dayStr3[] PROGMEM = " Wednesday";
            const char dayStr4[] PROGMEM = " Thursday";
            const char dayStr5[] PROGMEM = " Friday";
            const char dayStr6[] PROGMEM = " Saturday";
            const char dayStr7[] PROGMEM = " Sunday";

            const PPM dayNames_P[] =
            {
            (const PPM)dayStr0,
            (const PPM)dayStr1,
            (const PPM)dayStr2,
            (const PPM)dayStr3,
            (const PPM)dayStr4,
            (const PPM)dayStr5,
            (const PPM)dayStr6,
            (const PPM)dayStr7
            };

            Serial.print(F("day: "));
            Serial.println(dayOfWeek);
            Serial.println(dayNames_P[dayOfWeek]);

            strcat(pstrDateTime_OUT, dayNames_P[dayOfWeek]);
        }else{
            char buffer[4];
            strcat_P(pstrDateTime_OUT, (PGM_P)F(" Bogusday: "));
            strcat(pstrDateTime_OUT,itoa(dayOfWeek,buffer,10));
        }
    }
}

bool cDS3231::PrintTime(void)
{ 
  
  bool bResult;
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  if(ReadTime(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year)){
      //if(year == 1){ // in this case we DO display the error
          // since we are printing
      //   return false;
      //}
      char  pstrDateTime_OUT[64];
      
      FormatTimeS(pstrDateTime_OUT,second, minute, hour, dayOfWeek, dayOfMonth, month,year);
      
      // send it to therequiested port
      //Serial.print(F("Time/Date: "));

      cout << F("Time/Date: ");
      Serial.println(pstrDateTime_OUT);
      bResult = true;
  }else{
      Serial.println(F("DS3221 RTC Failure"));
      bResult = false;
  }
  return bResult;
}

cDS3231* pTime;

bool PrintTime(void){
    return pTime->PrintTime();
}

char GetDayOfWeek(void){
    char c;
    clearSerInput(Serial);
    
    cout << F(
         "\n"
         "Day of th e week must be an integer (Monday is 1, Sunday is 7):"
         "Enter day of week: ");

    while (!Serial.available()) {
        yield();
        }
    c = Serial.read();
    cout << c << endl;
    return c;
}

void GetTime(char*time){
  clearSerInput(Serial);
  
  cout << F("\nThe time must be in HH:MM:SS 24 hour format:"
        "\nEnter time: ");

  int idx=0;
  while(true){
      if (Serial.available() > 0) {
          time[idx] = Serial.read();
          cout << time[idx];
          idx++;
          if(idx == 8){
              time[8]='\0';
              break;
          }
      }
  }
  cout << F("\nYou Entered : ") << time << endl;
}

void GetDate(char* date){
  clearSerInput(Serial);
  
  cout << F("\n\nDate must be in MM/DD/YY format\nEnter date: ");

  while (!Serial.available()) {
      yield();
      }
  int idx=0;
  while(true){
      if (Serial.available() > 0) {
          date[idx] = Serial.read();
          cout << date[idx];
          idx++;
          if(idx == 8){
              date[8]='\0';
              break;
          }
      }
  }
  
  cout << F("\nYou Entered : ") << date << endl;
}

char GetOption(void){
  char c;

  clearSerInput(Serial);
  
  cout <<  F("\n"
      "'Set Clock' Options are:\n"
      "\tP - Print the DS3231's current date and time\n"
      "\tS - Set the DS3231 current date and time\n"
      "\tE - Exit. Board's Sketch will loop.\n"
      "\n"
      "Enter option: ");

  while (!Serial.available()) {
      yield();
      }
  c = Serial.read();
  cout << c << endl;
  return c;

}

//NOTE that caller must call Wire.begin() before calling this function
void DoSetDS3231(void){
  char c;
  //Start_AM2320();
  
  cout << F("\n\nAssuming a SDS3231 I2C address of 0x") ;
  Serial.println(DS3231_I2C_ADDRESS,HEX);
   
  cout << F("\nType any character to start: ");
  while (!Serial.available()) {
      yield();
  }
  bool exit = false;
  while(!exit){
      //cout << "\ncalling GetOption";
      c = GetOption();
      if ((c == 'e') || (c == 'E')) {
          cout << F("\nyou entered 'E'. Entering loop") << endl;
          break;
      }

      pTime = new cDS3231();
      
      if ((c=='p') || (c=='P')){
          int count=5;
          while(count--){
              //cout << "\ncalling PrintTime";
              Serial.print(F("Time/Date: ")); 
              pTime->PrintTime();
              delay(1000);
              //cout << "\nprinting time returned";
          }
          cout << ("\nPrinting time has completed.") << endl;

          continue;
      }
      
      if((c=='s') || (c=='S')){
          char date[9];
          char time[9];
          //char dayofweek;
          //char dayofmonth[2];
          int hour,minute,second,month,dom,dow,yy;
          while(true){

              GetDate(date);

              // validate date here IE 12/15/21
              // if not valid, continue.
              // valid is D1D2:D3D4:D5D6 where D is ascii numeric 0-9
              //where D1 is numeric ascii 0 or 1, D2 is numeric ascii 0-9
              //int i;
              
              date[2]= date[5] = '\0';
              month  = atoi(&date[0]);
              dom = atoi(&date[3]);
              yy = atoi(&date[6]);

              cout << F(" year: ") << yy << endl;
              
              if((month < 1) or month > 12){
                  cout << F("illegal month: ") << month << endl;
                  continue;     
              }
              int monthLimits[] = {0,31,0,31,30,31,30,31,31,30,31,30,31};

              int domlimit = monthLimits[month];
              if(!domlimit){
                  //month is february     
                  if(!(yy % 4)){
                      domlimit = 29;
                      }
                  else{
                      domlimit = 28;
                  }
              }
              
              if(dom > domlimit){
                  cout << F("\nillegal day of month: ") << month << endl;
                  continue;
                }

              if(yy < 01){
                  cout << F("\nillegal year: ") << yy << endl;
                  continue; 
              }
              break;
          }
          while(true){
              GetTime(time);
              time[2]= time[5] = '\0';
              hour = atoi(&time[0]);
              minute = atoi(&time[3]);
              second = atoi(&time[6]); 

              //cout << F(" second: ") << second;

              if((hour < 1) || hour > 23){
                  cout << F("\nillegal hour: ") << hour <<endl;
                  continue;
              }
              if((minute < 0) || minute > 59){
                  cout << F("\nillegal minute: ") << minute << endl;
                  continue;
              }
              if((second < 0) || second > 59){
                  cout << ("\nillegal second: " ) << second << endl;
                  continue;
              }
              break;
          }

          while(true){
              // convert data and time into integers to be used with
              char dayOfWeek[]={'\0','\0'};
              dayOfWeek[0]=GetDayOfWeek();
              dow = atoi(dayOfWeek);
              cout << F("\nday of week: ") << dow;
              if((dow < 1) || dow > 7){
                  cout << ("\nIllegal day of week: ") << dow << endl;
                  continue;
              }
              break;
          }
          char * daysOfWeek[] = {"NONE", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday","Sunday"};

          cout << F(" second: ")<< second;
          cout << F(" year: ")<< yy << endl;
          cout << F("\tday of week: " ) << dow <<    F(" (") << daysOfWeek[dow] <<  F(" )") << endl;
          while(true){
              cout << F("\n"
              "Do you want to set the clock? Options are \n"
              "\tY - Set the clock using the parameters above.\n"
              "\tN - Dont set the clock.\n"
              "\tE - Exit. Board's Sketch will loop.\n"
              "Enter option: ");

              while (!Serial.available()) {
                  yield();
                  }

              c = Serial.read();

              if((c == 'y') || (c == 'Y')){

                  cout << F("\n\nSetting the clock: ") << endl;
                  pTime->SetTime(second, //second
                      minute, //minute
                      hour,//hour
                      dow, //day of week MONDAY is 1, sunday is 7
                      dom, //day of month
                      month, //mopnth
                      yy); //year
                  exit=true;
                  cout << F("\n\nClock has been set: ") << endl;
                  break;

                  }
              else{
                  if ((c == 'e') ||(c == 'E')){
                      break;
                  }else{
                      if ((c == 'n') || (c == 'N')){
                          exit = true;
                          break; 
                      }else{
                          continue;
                      }

                  } 
              }
          } 
      }
    
  cout << F("\tcontinue...") << endl;
  }
  for( int i = 1;i++;i<=3){
      pTime->PrintTime();
      delay(1000);
  }
}
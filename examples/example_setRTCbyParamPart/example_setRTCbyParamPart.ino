#include "parampart_ex.h"
#include "HardwareSerial.h"
#include <DS3231Plus.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.6

Github: https://github.com/piotrku91/ParamPart/

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside,  
 use just Reaction function to easy work :)

Example:
- Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)
- Practical use interpreter for set RTC Timer and alarms.

Depedencies:
DS3231 - https://github.com/NorthernWidget/DS3231
DS3231Plus (My small Extension of DS3231) -
Wire - Library supports i2c

*/
//

DS3231Plus Clock;
ParamPart_Ex Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as parameter.

void DoSomething()
{
Serial.println("Snoring... Zzz....");  
};

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    Wire.begin();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reaction(ParamPart_Ex &P) // Access to ParamPart_Ex class by reference
{

    if (P.Header("delay")) // Delay
    {
        Clock.DelayRTCLock(5500,&DoSomething);
        P.ReadDone();
    };

    if ((P.Header("tset")) && P.EIntegrity(NUMBER,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER)) // Set time example
    {
      // YY;MM;DD;DW;HH;MM;SS;
      Clock.SetFullTime(P[0].toInt(),P[1].toInt(),P[2].toInt(),P[3].toInt(),P[4].toInt(),P[5].toInt(),P[7].toInt());
          
        P.ReadDone(true);
    };

       if (P.Header("aset_test")) // Set alarm test (One minute from now)
    {
      
      Clock.setA1Time(Clock.getDoW(), Clock.getHour(Clock.h12,Clock.PM), Clock.getMinute()+1, Clock.getSecond(), 0x0, true, false, false);
      Clock.turnOnAlarm(1);
 
      P.ReadDone(true);
    };


         if ((P.Header("tset")) && P.EIntegrity(NUMBER,NUMBER,NUMBER,NUMBER))  // Set alarm test 
    {
      
      Clock.setA1Time(P[3].toInt(), P[4].toInt(), P[5].toInt(), P[7].toInt(), 0x0, true, false, false);
      Clock.turnOnAlarm(1);
      P.ReadDone(true);


      
    };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop()
{

  if (Clock.checkIfAlarm(1)) { Serial.println("BOOOOOM ! :) Alarm executed! ");};
   
Serial.println(Clock.GetTimeString(true,false,true));
    delay(1000);
    Odczyt.HybridInterpreter(&Reaction);
    
}

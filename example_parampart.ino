// #include "parampart.h"
// #include "HardwareSerial.h"

// /* 
// Arduino Serial Data Splitter - ParamPart Example
// Written by Piotr Kupczyk (dajmosster@gmail.com) 
// 2019 - 2020
// v. 3.2

// Example:
// Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)



// */

// //

// void setup()
// {
//     Serial.begin(9600);
// }

// ParamPart OdczytBasic;

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void Reaction() 
// {

//     if ((OdczytBasic.Header("abc")) && OdczytBasic.Integrity(3,STRING,NUMBER,NUMBER))
//     {
//         Serial.println(OdczytBasic[0]);
//         OdczytBasic.Readed();
//     };

// };
// /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void loop()
// {
//     delay(150);

//     OdczytBasic << "<abc;rura;3;3;>";
//     Reaction();

// delay(1500);
   
  
// }
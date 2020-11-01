// #include "parampart.h"
// #include "HardwareSerial.h"

// /* 
// Arduino Serial Data Splitter - ParamPart_Ex Example
// Written by Piotr Kupczyk (dajmosster@gmail.com) 
// 2019 - 2020
// v. 2.4

// Example:
// Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

// Send to Serial: <name;Piotr;23;167;> 
// You get respond: Czesc Piotr, podobno masz 23 lat oraz 167 wzrostu.


// */

// //

// void setup()
// {
//     Serial.begin(9600);
// }

// ParamPart OdczytBasic;

// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void Reaction() // In this version you need to checks and execute everything and by yourself (SyntaxVerify,Debug,Print by serial)
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
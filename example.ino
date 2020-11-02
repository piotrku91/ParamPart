#include "parampart_ex.h"
#include "HardwareSerial.h"

/* 
Arduino Serial Data Splitter - ParamPart_Ex Example
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.2

Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <name;Piotr;23;167;> 
You get respond: Czesc Piotr, podobno masz 23 lat oraz 167 wzrostu.


*/

//

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN,OUTPUT);
}

ParamPart_Ex Odczyt(&Serial);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reaction(ParamPart_Ex& P) // Access to ParamPart_Ex class by reference
{

    if ((P.Header("abc")) && P.Integrity(3, STRING, NUMBER, NUMBER))
    {
        P.pnt_Serial->print("Hi ");
        P.pnt_Serial->print(P[0]); // [] is overloaded, so you can use P[0] instead of P.Params[0].
        P.pnt_Serial->print(", you have ");
        P.pnt_Serial->print(P[1]);
        P.pnt_Serial->print(" years old and ");
        P.pnt_Serial->print(P[2]);
        P.pnt_Serial->println(" cm.  ");
        P.Readed();
    };

    if ((P.Header("dw")) && P.Integrity(1,NUMBER))
    {
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.Readed();
    };

     if ((P.Header("test")) && (P.Integrity(1, NUMBER))) //Example of comparition and return score.
    {
        digitalWrite(LED_BUILTIN, P[0].toInt());
        P.Readed();
    };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(100);
 
    Odczyt.HybridInterpreter(&Reaction);

   
  //  Serial.println("siema");
}
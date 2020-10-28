#include "parampart_ex.h"
#include "HardwareSerial.h"

/* 
Arduino Serial Data Splitter - ParamPart_Ex Example
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4

Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <name;Piotr;23;167;> 
You get respond: Czesc Piotr, podobno masz 23 lat oraz 167 wzrostu.


*/

//

void setup()
{
    Serial.begin(9600);
}

ParamPart_Ex Odczyt(&Serial);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reaction(ParamPart_Ex *P) // Access to ParamPart_Ex class by pointer
{

    if ((P->Header("<name")) && (P->Integrity(3, STRING, NUMBER, NUMBER)))
    {
        P->pnt_Serial->print("Hi ");
        P->pnt_Serial->print(P->Params[0]);
        P->pnt_Serial->print(", you have ");
        P->pnt_Serial->print(P->Params[1]);
        P->pnt_Serial->print(" years old and ");
        P->pnt_Serial->print(P->Params[2]);
        P->pnt_Serial->println(" cm.  ");
        P->Readed();
    };


    if ((P->Header("<dbg")) && (P->Integrity(1, NUMBER))) //Example of change Debug mode status
    {
        if (atoi(P->Params[0]))
        {
            P->pnt_Serial->println("Debug on. ");
            P->DebugEnabled = true;
        }
        else
        {
            P->pnt_Serial->println("Debug off. ");
            P->DebugEnabled = false;
        };
        P->Readed();
    };



};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(1000);
 
    Odczyt.HybridRead(&Reaction);
    delay(1000);
}
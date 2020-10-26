#include "parampart_ex.h"
#include "HardwareSerial.h"

/* 
Arduino Serial Data Splitter - ParamPart_Ex Example
Created by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4

Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <name;Piotr;23;167;> 
You get respond: Czesc Piotr, podobno masz 23 lat oraz 167 wzrostu.


*/ 

#define TEKST 1
#define LICZBA 0

void setup()
{
    Serial.begin(9600);
}

ParamPart_Ex Odczyt(&Serial);

//////////////////////////////////////////////////////// Funkcja reagująca ////////////////////////////////////////////////////////
void Reaction(ParamPart_Ex *P) // Access to ParamPart_Ex class by pointer
{
    if (P->SyntaxVerify())
    { // Jeżeli skłádnia OK (SYNTAX OK)

        if ((P->Header("<name")) && (P->Integrity(3, TEKST, LICZBA, LICZBA)))
        {
            P->pnt_Serial->print("Czesc ");
            P->pnt_Serial->print(P->Params[0]);
            P->pnt_Serial->print(", podobno masz ");
            P->pnt_Serial->print(P->Params[1]);
            P->pnt_Serial->print(" lat oraz ");
            P->pnt_Serial->print(P->Params[2]);
            P->pnt_Serial->println("cm wzrostu.");
            P->Clear();
        };
    }
    else
    { //Jeżeli składnia jest niepoprawna (SYNTAX ERROR)

        P->pnt_Serial->println("Błąd składni (Syntax Error)");
    }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(1000);

    Odczyt.HybridRead(&Reaction);
    delay(1000);
    Odczyt.Clear();
}
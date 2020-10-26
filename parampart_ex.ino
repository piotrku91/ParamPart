#include "parampart_ex.h";

/* 
Arduino Serial Data Splitter - ParamPart_Ex (Extended Version with Serial Receiver)
Created by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4
*/ 

void ParamPart_Ex::SerialChecker(char newLine[])
{
    String Dane; // tmp string

    strcpy(newLine, "#"); // linie ignorowane

    while (pnt_Serial->available())
    {
        Dane = pnt_Serial->readStringUntil(NULL);
        strcpy(newLine, Dane.c_str());
    };
};

void ParamPart_Ex::HybridRead(void (*ptn_func_interpreter)(ParamPart_Ex *PP))
{
    char tmpnewLine[64];
    SerialChecker(tmpnewLine);

    if (tmpnewLine[0] != '#')
    {
        Slicer(tmpnewLine);
        // pnt_Serial->println(tmpnewLine); // DEBUG LINE
        (*ptn_func_interpreter)(this); // Uruchamia funkcje CALLBACK, która zareaguje na dane. Dostęp do Obiektu ParamPart_Ex przez wskaźnik w argumencie funkcji.
    }

    /* 
Przykład funkcji w programie:
void Reaction(ParamPart_Ex * P){Serial.println(P->Params[0])};

Przykład wywołania:
Odczyt.HybridRead(&Reaction);
*/
};
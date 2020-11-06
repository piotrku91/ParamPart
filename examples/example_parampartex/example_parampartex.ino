#include "parampart_ex.h"
#include "HardwareSerial.h"

/* 
Arduino Serial String Data Splitter  - ParamPart_Ex Example
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.1

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside,  
 use just Reaction function to easy work :)

Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <abc;peter;23;167;> 
You get respond: Hi Peter, you have 33 years old and 160 cm.  
<artn;abc;OK;>

*/

//

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
}

ParamPart_Ex Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as parameter.

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reaction(ParamPart_Ex &P) // Access to ParamPart_Ex class by reference
{

    if ((P.Header("abc")) && P.Integrity(3, STRING, NUMBER, NUMBER))
    {
        P.pnt_Serial->print("Hi ");
        P.pnt_Serial->print(P[0]); // [] is overloaded, so you can use P[0] instead of P.GetParam(0).
        P.pnt_Serial->print(", you have ");
        P.pnt_Serial->print(P[1]);
        P.pnt_Serial->print(" years old and ");
        P.pnt_Serial->print(P[2]);
        P.pnt_Serial->println(" cm.  ");

        P.ReadDone(); // (bool RtnMsg, String ParamRtn, String Rtn) - You can configure return output (true or false, return data, return name of executed command)
                      // Always use this function after finish your reaction block. It's setting ReadFlag.
    };

    if ((P.Header("dw")) && P.Integrity(1, NUMBER)) // Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.ReadDone();
    };

    if ((P.Header("db")) && P.Integrity(1, NUMBER)) // Change debug mode
    {

        P.SetDebugMode(P[0].toInt());
        P.ReadDone();
    };

    if ((P.Header("cmp")) && P.Integrity(2, NUMBER, NUMBER)) // Compare and send sum by return example
    {
        if (P[0] > P[1])
            Serial.print("1st");
        if (P[0] < P[1])
            Serial.print("2nd");
        if (P[0] == P[1])
            P.pnt_Serial->println("same");
        else
            P.pnt_Serial->println(" is bigger");

        int sum = (P[0].toInt() + P[1].toInt()); // Do some example maths and return as parameter.
        P.ReadDone(true, static_cast<String>(sum));
    };
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(100);

    Odczyt.HybridInterpreter(&Reaction);
}

#include "parampart_ex.h"
#include "parampart_ex.cpp" // Include cpp file as well for create and link all functions from library.

//#include "SoftwareSerial.h" // For use ParamPart_Ex with Software Serial

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2021
v. 3.5

Github: https://github.com/piotrku91/ParamPart/

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside,  
 use just Reaction function to easy work :)

Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <abc;peter;23;167;> 
You get respond: Hi Peter, you have 33 years old and 160 cm.  
<artn;abc;OK;>

*/

//SoftwareSerial SerialSoft(6,7,false); // Create object for use SoftwareSerial. (Remember to uncomment include of header file)

void setup()
{
    // Serial start...

    // HardwareSerial
    Serial.begin(9600); // Open Serial Port

    // SoftwareSerial
    // SerialSoft.begin(9600); // Open Serial Port

    pinMode(LED_BUILTIN, OUTPUT); // Set led to OUTPUT for use in this example
}

// Examples of create ParamPart_Ex object:
ParamPart_Ex<> Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as parameter. (Default is HardwareSerial)
//ParamPart_Ex <HardwareSerial> Odczyt(&Serial); // Equal to ParamPart_Ex <> Odczyt(&Serial);
//ParamPart_Ex <SoftwareSerial> Odczyt2(&SerialSoft); // Create object ParamPart_Ex with pointer to Serial as parameter. (SoftwareSerial version)

//ParamPart_Ex <> Odczyt(&Serial,'@',';','!'); // Version with overloaded syntax (open line, delimiter, close line)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void Reaction(ParamPart_Ex <SoftwareSerial> &P) // Access to ParamPart_Ex object by reference (SoftwareSerial)
void Reaction(ParamPart_Ex<> &P) // Access to ParamPart_Ex object by reference (HardwareSerial)
{
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("test")) // Simple example of respond
    {
        P.ptr_Serial->println("Hi!");
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("test2"), false) // Simple example of respond (If second argument is false, this block of reaction is deactivated)
    {
        P.ptr_Serial->println("Hi!");
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////

    if (P.Header("js")) // Example of export ParamPart parameters to JSON format
    {
        P.ptr_Serial->println(P.toJSON());
        P.readDone();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("abc")) && P.Integrity(PT::Txt, PT::Num, PT::Num))
    {
        P.ptr_Serial->print("Hi ");
        P.ptr_Serial->print(P[0]); // [] is overloaded, so you can use P[0] instead of P.getParam(0).
        P.ptr_Serial->print(", you have ");
        P.ptr_Serial->print(P[1]);
        P.ptr_Serial->print(" years old and ");
        P.ptr_Serial->print(P[2]);
        P.ptr_Serial->println(" cm.  ");

        for (auto &pa : P)
        {
            P.ptr_Serial->println(pa);
        }; // Print all parameters by range-based loop.

        P.readDone(); // (bool RtnMsg, String ParamRtn, String Rtn) - You can configure return output (true or false, return data, return name of executed command)
                      // Always use this function after finish your reaction block. It's setting ReadFlag.
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("dw")) && P.Integrity(PT::Num)) // Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P("db", true, PT::Num)) // Change debug mode
                                // (Example of use short version - overloaded () for ParamPart object. Arguments: Command, Status of active command, Expected parameters).
    {

        P.setDebugMode(P[0].toInt());
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("cmp")) && P.Integrity(PT::Num, PT::Num)) // Compare and send sum by return example (Expanded version test)
    {
        if (P[0] > P[1])
            Serial.print("1st");
        if (P[0] < P[1])
            Serial.print("2nd");
        if (P[0] == P[1])
            P.ptr_Serial->println("same");
        else
            P.ptr_Serial->println(" is bigger");

        int sum = (P[0].toInt() + P[1].toInt()); // Do some example maths and return as parameter.
        P.readDone(true, static_cast<String>(sum));
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(100);
    Odczyt.HybridInterpreter(&Reaction); // Read and react to data from Serial.
}
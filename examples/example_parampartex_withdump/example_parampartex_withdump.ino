#include "parampart_ex.h"
#include "HardwareSerial.h"

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2021
v. 3.4.3

Github: https://github.com/piotrku91/ParamPart/

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside,  
 use just Reaction function to easy work :)

Example:
- Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)
- Dump used commands in Reaction function ( I needed this for my project where I had around 100 reaction blocks and I lost :) )

Send to Serial: <abc;peter;23;167;> 
You get respond: Hi Peter, you have 33 years old and 160 cm.  
<artn;abc;OK;>

*/

//

  
  void FoundCmd(const String& Cmd) //Example of function which is listing implemented commands (Headers).
  {
    static int Count=1;
    Serial.print(Count);
    Serial.print(" - Found command: ");
    Serial.println(Cmd);
    
    // Save to some file function etc....
    
    Count++;
    
  };

  ParamPart_Ex <> Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as parameter.

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
  //  P.SetExportFunction(&FoundCmd); // Export from the begin (Starts when you send first String to interpreter.)
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Reaction(ParamPart_Ex <> &P) // Access to ParamPart_Ex class by reference
{

      if (P.Header("ex"))  // Export implemented commands (Headers). Should be first block in Reaction function to dump everything.
    {
        P.SetExportFunction(&FoundCmd); // Execute function which is listing implemented commands (Headers)
        P.ReadDone(false);
    };



    if ((P.Header("abc")) && P.Integrity(3,STRING, NUMBER, NUMBER))
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

    if ((P.Header("dw")) && P.Integrity(1,NUMBER)) // Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.ReadDone();
    };


};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    delay(100);
    Odczyt.HybridInterpreter(&Reaction);
}

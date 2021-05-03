  #include "parampart.h"
  #include "HardwareSerial.h"

  /* 
  Arduino Serial Serial Data Splitter - ParamPart Basic Example
  Written by Piotr Kupczyk (dajmosster@gmail.com) 
  2019 - 2021
  v. 3.5

 In this version you need to pass String to ParamPart Object manualy from Serial or Script file, whatever you want :) 
 
  Example:
  Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to ParamPart: <abc;peter;23;167;> ( from Serial or Script file, whatever you want :) )
You get respond: Hi Peter, you have 33 years old and 160 cm.  
<artn;abc;OK;>

  */
 

  void setup()
  {
      Serial.begin(9600);
  }

  ParamPart OdczytBasic;

  void Reaction(ParamPart& P) 
  {

    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("test")) // Simple example of respond
    {
         Serial.println("Hi!");
        P.ReadDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("test2"), false) // Simple example of respond (If second argument is false, this block of reaction is deactivated)
    {
         Serial.println("Hi!");
        P.ReadDone();
    };
     ////////////////////////////////////////////////////////////////////////////////////////////////

        if (P.Header("js")) // Example of export ParamPart parameters to JSON format
    {
        Serial.println(P.toJSON());
        P.ReadDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
       if ((P.Header("abc")) && P.Integrity(3, PT::Txt, PT::Num, PT::Num))
    {
        Serial.print("Hi ");
        Serial.print(P[0]); // [] is overloaded, so you can use P[0] instead of P.GetParam(0).
        Serial.print(", you have ");
        Serial.print(P[1]);
        Serial.print(" years old and ");
        Serial.print(P[2]);
        Serial.println(" cm.  ");

         for (auto& pa : P) { Serial.println(pa); }; // Print all parameters by range-based loop.
        
        P.ReadDone(); // (bool RtnMsg, String ParamRtn, String Rtn) - You can configure return output (true or false, return data, return name of executed command)
                    // Always use this function after finish your reaction block. It's setting ReadFlag.
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
       if (P("db", true, PT::Num)) // Change debug mode 
    // (Example of use short version - overloaded () for ParamPart object. Arguments: Command, Status of active command, Expected parameters).
     {

        P.SetDebugMode(P[0].toInt());
        P.ReadDone();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("dw")) && P.Integrity(PT::Num))  //Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.ReadDone();
    }; 
    ////////////////////////////////////////////////////////////////////////////////////////////////



  };
 

  void loop()
  {
      delay(150);
       // Example with Serial: if (Serial.available()) {String txt=Serial.readStringUntil(0);}; OdczytBasic << txt; 

      OdczytBasic << "<abc;peter;33;223;>"; // Push some line inside from Serial, i2c, variable, file etc. (<< overloaded - instead of Slicer(String&)) 
      
      
      String ScoreFlag = OdczytBasic.Interpreter(&Reaction); // Read by reaction function and read the return flag.
      if (ScoreFlag!="") Serial.println(ScoreFlag); // If flag is empty it is mean NO ERRORS of syntax and integrity checks.

  delay(1500);
   
  
  }

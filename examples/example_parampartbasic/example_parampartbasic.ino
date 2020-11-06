  #include "parampart.h"
  #include "HardwareSerial.h"

  /* 
  Arduino Serial Serial Data Splitter - ParamPart Basic Example
  Written by Piotr Kupczyk (dajmosster@gmail.com) 
  2019 - 2020
  v. 3.3.1

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

       if ((P.Header("abc")) && P.Integrity(3, STRING, NUMBER, NUMBER))
    {
        Serial.print("Hi ");
        Serial.print(P[0]); // [] is overloaded, so you can use P[0] instead of P.GetParam(0).
        Serial.print(", you have ");
        Serial.print(P[1]);
        Serial.print(" years old and ");
        Serial.print(P[2]);
        Serial.println(" cm.  ");
        
        P.ReadDone(); // (bool RtnMsg, String ParamRtn, String Rtn) - You can configure return output (true or false, return data, return name of executed command)
                    // Always use this function after finish your reaction block. It's setting ReadFlag.
    };

    if ((P.Header("dw")) && P.Integrity(1,NUMBER))  //Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.ReadDone();
    }; 
 



  };
 

  void loop()
  {
      delay(150);
       // Example with Serial: if (Serial.available()) {String txt=Serial.readStringUntil(0);}; OdczytBasic << txt; 
      OdczytBasic << "<abc;peter;33;223;>"; // Push some line inside from Serial, variable, file etc. (<< overloaded - instead of Slicer(String&)) 
      
      
      String ScoreFlag = OdczytBasic.Interpreter(&Reaction); // Read by reaction function and read the return flag.
      if (ScoreFlag!="") Serial.println(ScoreFlag); // If flag is empty it is mean NO ERRORS of syntax and integrity checks.

  delay(1500);
   
  
  }
# ParamPart
Arduino Serial String Data Splitter  - ParamPart / ParamPart_Ex

Written by Piotr Kupczyk (dajmosster@gmail.com) 

2019 - 2020
v. 3.3

**It's easy to put in your prototype project and debug, send, receiver parameters by string lines.**




Example:
Send to Serial: <abc;peter;23;167;> 

You get respond: **Hi peter, you have 23 years old and 167 cm.** (Instead of this you can do something)
and get some return <artn;abc;OK;>

All data is stored in **ParamPart Object** (in params P[0],P[1]).

And you can do something by Reaction function:

>

       void Reaction(ParamPart& P) 
       {

       if ((P.Header("abc")) && P.Integrity(3, STRING, NUMBER, NUMBER))
    {
        Serial.print("Hi ");
        Serial.print(P[0]); // [] is overloaded, so you can use P[0] instead of P.Params[0].
        Serial.print(", you have ");
        Serial.print(P[1]);
        Serial.print(" years old and ");
        Serial.print(P[2]);
        Serial.println(" cm.  ");
        
        P.Readed(); 
    };

    };





Two libraries:

ParamPart_Ex - Extended version with Serial control.

ParamPart - Normal basic version with no Serial control.


## ParamPart_Ex

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside. 
 Use just Reaction function in your program to easy work :)
 
Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <abc;peter;23;167;> 

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>


## ParamPart

In this version you need to pass String to ParamPart Object manualy from Serial or Script file, whatever you want :) 
Use just Reaction function in your program to easy work or do everything by yourself (more configurable).

  Example:
  Fast implementation of parsing the data, integrity checks and reaction :)
  
Send to ParamPart: <abc;peter;23;167;> ( from Serial or Script file, whatever you want :) )

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>


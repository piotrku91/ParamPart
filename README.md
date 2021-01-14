# ParamPart
Arduino Serial String Data Splitter  - ParamPart / ParamPart_Ex

Written by Piotr Kupczyk (dajmosster@gmail.com) 

Tested on Arduino Uno and Arduino Mega.
Used IDE: Arduino IDE 1.8.13 / Visual Studio Code 1.50.1 (Linux)

2019 - 2020
v. 3.3.8

**It's easy to put in your prototype project and debug, send, receive parameters by string lines.**


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
        Serial.print(P[0]); // [] is overloaded, so you can use P[0] instead of P.GetParam(0).
        Serial.print(", you have ");
        Serial.print(P[1]);
        Serial.print(" years old and ");
        Serial.print(P[2]);
        Serial.println(" cm.  ");
        
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
            Serial.println("same");
        else
            Serial.println(" is bigger");

        int sum = (P[0].toInt() + P[1].toInt()); // Do some example maths and return as parameter.
        P.ReadDone(true, static_cast<String>(sum));
    };
    };





Three libraries:

ParamPart_Ex - Extended version with Serial control. (parampart_ex.h)

ParamPart - Normal basic version with no Serial control. (parampart.h)

ParamPart (PCS) - Normal basic version to use on PC side (port). (Is in another repository)


## ParamPart_Ex

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside. 
 Use just Reaction function in your program to easy work :)
 
Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <abc;peter;23;167;> 

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>

**Creating object**

ParamPart_Ex Reader(&Serial) - That's need pointer to Serial object to work properly.

ParamPart_Ex Reader(&Serial,'<',';','>') - You can customize syntax by yourself from the begin (or later by function)


## ParamPart

In this version you need to pass String to ParamPart Object manualy from Serial or Script file, whatever you want :) 
Use just Reaction function in your program to easy work or do everything by yourself (more configurable).

  Example:
  Fast implementation of parsing the data, integrity checks and reaction :)
  
Send to ParamPart: <abc;peter;23;167;> ( from Serial, i2c, Script file, whatever you want :) )

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>

**Creating object**

ParamPart Reader('<',';','>') - You can customize syntax by yourself from the begin (or later by function)


## Main functions (ParamPart_Ex)

**String HybridInterpreter(void (*ptn_func_interpreter)(ParamPart &PP))** - Read String from Serial, send to Slice() function, checks syntax and later call function from pointer (Reaction function).

**void Interpreter(void (*ptn_func_interpreter)(ParamPart &PP))**  - Checks syntax, and call function from pointer. (Reaction function)

**String RawRead()** - Read String from Serial and store in tmpnewLine variable. (No interpreting)

**void ReadDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn")** - Set ReadFlag as true, and create some return message (In this version it's going straight away to Serial)


## Main functions (ParamPart)

**String Interpreter(void (*ptn_func_interpreter)(ParamPart &PP))**  - Checks syntax, and call function from pointer. But in this version function returns String with Flag about reaction process or nothing if everything is OK. (Reaction function)

**void ReadDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn")** - Set ReadFlag as true, and create some return message (In this version function returns only string)


## Main functions (both)

**bool Header(const String& CmdName,bool& Active)** - Checks if input Command is one of defined Reaction blocks command.

**bool Slicer(String& LineS)** - Parse String by designated syntax and store in ParamPart object.

**bool CSlicer(char Line[])** - Parse CString by designated syntax and store in ParamPart object. (Old version of function).

**bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0)** - Check amount of parameters, data types inside and compare with expected types. 

**  bool Integrity(uint8_t InputExpectedParams = 0, bool Type1 = 0, bool Type2 = 0, bool Type3 = 0, bool Type4 = 0, bool Type5 = 0, bool Type6 = 0, bool Type7 = 0, bool Type8 = 0, bool Type9 = 0); ** - Check types of input and expected parameters. Return false if is any mismatch. 

**  template<typename... TPack> bool EIntegrity(TPack&&... args) ** - Check types of input and expected parameters. Return false if is any mismatch. Expanded version of Integrity, It's possible now to use more than 9 arguments (and Max is set by MAX_PARAMS in parampart header file). Don't need specify of amount expected parameters. 

**  String Glue(); ** - Create String line from actual object variables (inversion).

**String Glue()** - Create String line from actual object variables (inversion).

**Clear()** - Clean object and prepare to next String line.


## Return Flags after Reaction() function.

**SE** - Syntax Error

**MEP** - Missing/Extra Parameter

**UC** - Unknown Command

**MM** - Mismatch between received and expected parameters.

**Nothing** - Everything OK :)



## Settings and other functions

**SetDebugMode(bool DebugStatus)** - Enable/Disable debug return.

**SetIntegrityCheck(bool IntegrityStatus)** - Enable/Disable Integrity checks. (Pass everything)

**SetSyntaxChars(char OpenLine, char Delimiter, char CloseLine)** - Customize your syntax as you like

**bool GetReadFlag()** - Returns if some reaction is done.

**bool GetParam(n)** - Returns parameter with index n (it's same like in example: P[n])

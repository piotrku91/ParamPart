# ParamPart
Arduino Serial String Data Splitter  - ParamPart / ParamPart_Ex

Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 

Tested on Arduino Uno and Arduino Mega.
Used IDE: Arduino IDE 1.8.13 / Visual Studio Code 1.50.1 (Linux)

2019 - 2021
v. 3.7

**It's easy to put in your prototype project and debug, send, receive parameters by string lines.**


Example:
Send to Serial: <abc;peter;23;167;> 

You get respond: **Hi peter, you have 23 years old and 167 cm.** (Instead of this you can do something)
and get some return <artn;abc;OK;>

All data is stored in **ParamPart Object** (in params P[0],P[1]).

Integity you can check by pass enums from PT range (PT::Num - for numeric argument, PT::Txt - for string argument, PT::Any - type not important) to Integrity() function. Check below.

And you can do something by Reaction function:

>

       void Reaction(ParamPart &P)
    {
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("test")) // Simple example of respond
    {
        Serial.println("Hi!");
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P.Header("js")) // Example of export ParamPart parameters to JSON format
    {
        Serial.println(P.toJSON());
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("abc")) && P.Integrity(PT::Txt, PT::Num, PT::Num))
    {
        Serial.print("Hi ");
        Serial.print(P[0]); // [] is overloaded, so you can use P[0] instead of P.getParam(0).
        Serial.print(", you have ");
        Serial.print(P[1]);
        Serial.print(" years old and ");
        Serial.print(P[2]);
        Serial.println(" cm.  ");

        for (auto &pa : P)
        {
            Serial.println(pa);
        }; // Print all parameters by range-based loop.

        P.readDone(); // (bool RtnMsg, String ParamRtn, String Rtn) - You can configure return output (true or false, return data, return name of executed              command)
                      // Always use this function after finish your reaction block. It's setting ReadFlag.
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
    if (P("db", true, PT::Num)) // Change debug mode
                                // (Example of use short version - overloaded () for ParamPart object. Arguments: m_Command, Status of active command, Expected parameters).
    {
        P.setDebugMode(P[0].toInt());
        P.readDone();
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    if ((P.Header("dw")) && P.Integrity(PT::Num)) //Simple digitalWrite example
    {
        // If pass integrity checks, i'm not scared to conversion P[0] to int.
        digitalWrite(LED_BUILTIN, (P[0].toInt()));
        P.readDone();
    };
    ////////////////////////////////////////////////////////////////////////////////////////////////
       };

Some examples and more information you can find on my YouTube channel: https://www.youtube.com/watch?v=d5QSMJFkmyw



Three libraries:

ParamPart_Ex - Extended version with Serial control. (parampart_ex.h)

ParamPart - Normal basic version with no Serial control. (parampart.h)

ParamPart (PCS) - Normal basic version to use on PC side (port). It is compatibility to use with Qt (It is in another branch).


## ParamPart_Ex

 In this version (ParamPart_Ex) you need to pass just pointer to Serial object and everything is controlled inside. 
 Use just Reaction function in your program to easy work :)
 
Example:
Fast implementation of Serial receiving, parsing the data, integrity checks and reaction :)

Send to Serial: <abc;peter;23;167;> 

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>

**Creating object**

ParamPart_Ex is template which allow to build object with internal support of Serial. It takes type of Serial type (Hardware/Software) as template argument.

Examples of create ParamPart_Ex object:

ParamPart_Ex <> Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as constructor argument and HardwareSerial as template argument. (Default is HardwareSerial)

ParamPart_Ex < HardwareSerial > Odczyt(&Serial);  // Equal to ParamPart_Ex <> Odczyt(&Serial);

ParamPart_Ex < SoftwareSerial > Odczyt2(&SerialSoft); // Create object ParamPart_Ex with pointer to Serial as constructor argument and SoftwareSerial as template argument. (SoftwareSerial version)

ParamPart_Ex <> Odczyt(&Serial,'@',';','!'); // Version with overloaded syntax (open line, delimiter, close line)

## ParamPart

In this version you need to pass String to ParamPart Object manualy from Serial or Script file, whatever you want :) 
Use just Reaction function in your program to easy work or do everything by yourself (more configurable).

  Example:
  Fast implementation of parsing the data, integrity checks and reaction :)
  
Send to ParamPart: <abc;peter;23;167;> ( from Serial, i2c, Script file, whatever you want :) )

You get respond: Hi peter, you have 23 years old and 167 cm.  (Instead of this you can do something)
and get some return <artn;abc;OK;>

**Creating object**

**ParamPart Reader;** // Basic version of ParamPart object. Ready to work.

**ParamPart Reader('<',';','>')** - // Version with overloaded syntax (open line, delimiter, close line)


## Main functions (ParamPart_Ex)

**String HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex <> &PP))** - Read String from Serial, send to Slice() function, checks syntax and later call function from pointer (Reaction function).

**void Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex <> &PP))**  - Checks syntax, and call function from pointer. (Reaction function)

**String RawRead()** - Read String from Serial and store in m_tmpnewLine variable. (No interpreting)

**void readDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn")** - Set ReadFlag as true, and create some return message (In this version it's going straight away to Serial)


## Main functions (ParamPart)

**String Interpreter(void (*ptn_func_interpreter)(ParamPart &PP))**  - Checks syntax, and call function from pointer. But in this version function returns String with Flag about reaction process or nothing if everything is OK. (Reaction function)

**void readDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn")** - Set ReadFlag as true, and create some return message (In this version function returns only string)


## Main functions (both)

**bool Header(const String& CmdName,bool& Active)** - Checks if input m_Command is one of defined Reaction blocks command.

**bool Slicer(String& LineS)** - Parse String by designated syntax and store in ParamPart object.

**bool CSlicer(char Line[])** - Parse CString by designated syntax and store in ParamPart object. (Old version of function).

**  template<typename... TPack> bool Integrity(TPack&&... args) ** - Check types of input and expected parameters. Don't need specify of amount expected parameters. 

**String Glue()** - Create String line from actual object variables (inversion).

**String toJSON()** - Returns parameters stored in ParamPart in JSON format.

**Clear()** - Clean object and prepare to next String line.


## Return Flags after Reaction() function.

**SE** - Syntax Error

**MEP** - Missing/Extra Parameter

**UC** - Unknown m_Command

**MM** - Mismatch between received and expected parameters.

**Nothing** - Everything OK :)



## Settings and other functions

**setDebugMode(bool DebugStatus)** - Enable/Disable debug return.

**setIntegrityCheck(bool IntegrityStatus)** - Enable/Disable Integrity checks. (Pass everything)

**setExportFunction(*External_Export_func)(const String&))** - Enable function for dump all reaction commands. After Reaction function is reset to nullptr.

**setSyntaxChars(char m_OpenLine, char Delimiter, char m_CloseLine)** - Customize your syntax as you like

**bool getReadFlag()** - Returns if some reaction is done.

**bool getParam(n)** - Returns parameter with index n (it's same like in example: P[n])

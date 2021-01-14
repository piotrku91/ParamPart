#ifndef PARAMPART_EX_H
#define PARAMPART_EX_H
#include "parampart.h"
#include <Arduino.h>
/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.3.8

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart_Ex  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParamPart_Ex : public ParamPart // Extended Arduino Serial Data Splitter
{
public:
    HardwareSerial *pnt_Serial;

    void HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex& PP));
    void Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex& PP));
    String RawRead();
    void ReadDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");

    // Constructors for extended class
    ParamPart_Ex() = delete; // disable default constructor
    ParamPart_Ex(HardwareSerial *WS, char OL = '<', char DL = ';', char CL = '>') : 
      ParamPart(OL, DL, CL), pnt_Serial(WS){};


      template <typename T> 
void Interpreter(T *M,void (T::*ptn_func_interpreter)(ParamPart &PP)) 
{

    if (SyntaxVerify())
    {                                  //   (SYNTAX OK)
        (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
        if (!(Export_func==nullptr)) UnSetExportFunction();
        if ((DebugEnabled) && (DebugIntegrityDump != ""))
            pnt_Serial->println(DebugIntegrityDump); // Debug Integrity error print (if is ok, nothing to print)
        if ((DebugEnabled) && (!GetReadFlag() && (DebugIntegrityDump == "")))
            pnt_Serial->println("UC! (" + Command + ")"); // Unknown command print
        Clear();                                          // Clear parampart to prepare for the next input.
    }
    else
    { // (SYNTAX ERROR)

        if ((DebugEnabled))
            pnt_Serial->println("SE!"); // Syntax Error - missing < or ; or >
    };
};

};



#endif

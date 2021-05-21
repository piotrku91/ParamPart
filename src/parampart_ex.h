#pragma once
#include "parampart.h"
#include <Arduino.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.6

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart_Ex  - Header file  (Template)                                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ParamPart_Ex <HardwareSerial> Odczyt(&Serial); // Create object ParamPart_Ex with pointer to Serial as parameter. (Default is HardwareSerial). Equal to ParamPart_Ex <> Odczyt(&Serial);
//ParamPart_Ex <SoftwareSerial> Odczyt2(&SerialSoft); // Create object ParamPart_Ex with pointer to Serial as parameter. (SoftwareSerial version)
template <typename SerialType = HardwareSerial>
class ParamPart_Ex final : public ParamPart // Extended Arduino Serial Data Splitter
{

public:
    SerialType *ptr_Serial; // Pointer to Serial Object (HardwareSerial/SoftwareSerial)

    // Overloaded functions from basic version
    void HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP));
    void Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP));
    String RawRead();
    void readDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");

    // Constructors for extended class
    ParamPart_Ex(ParamPart_Ex<SerialType> &) = delete; // disable copy constructor
    ParamPart_Ex() = delete;                           // disable default constructor

    ParamPart_Ex(SerialType *WS, char OL = '<', char DL = ';', char CL = '>') : ParamPart(9, OL, DL, CL), ptr_Serial(WS){};
    ParamPart_Ex(SerialType *WS, int size) : ParamPart(size), ptr_Serial(WS){};

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*                                      Special template for Reaction Function from class member

  CALL EXAMPLE:
  For example - Reaction function is a member of MainSetter class (MainSetter::Reaction).

  typedef void (MainSetter::*ReactPTR)(ParamPart &PP);
  ReactPTR MemberReactionPointer= &MainSetter::Reaction;
  OdczytParted.Interpreter(this,MemberReactionPointer);


*/
    template <typename T>
    void Interpreter(T *M, void (T::*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP))
    {

        if (syntaxVerify())
        {                                      //   (SYNTAX OK)
            (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
            if (!(Export_func == nullptr))
                unsetExportFunction();
            if ((DebugEnabled) && (DebugIntegrityDump != ""))
                ptr_Serial->println(DebugIntegrityDump); // Debug Integrity error print (if is ok, nothing to print)
            if ((DebugEnabled) && (!Getm_ReadFlag() && (DebugIntegrityDump == "")))
                ptr_Serial->println("UC! (" + Command + ")"); // Unknown command print
            Clear();                                          // Clear parampart to prepare for the next input.
        }
        else
        { // (SYNTAX ERROR)

            if ((DebugEnabled))
                ptr_Serial->println("SE!"); // Syntax Error - missing < or ; or >
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

#include "parampart_ex.cpp" // Include functions file to link everything properly.



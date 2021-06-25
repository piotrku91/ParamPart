#pragma once
#include "parampart.h"
#include <Arduino.h>

/* 
Arduino Serial String Data Splitter - ParamPart
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.7

Github: https://github.com/piotrku91/ParamPart/
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamPart_Ex  - Header file  (Template)                                                                   //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ParamPart_Ex <HardwareSerial> Odczyt(&Segit corial); // Create object ParamPart_Ex with pointer to Serial as parameter. (Default is HardwareSerial). Equal to ParamPart_Ex <> Odczyt(&Serial);
//ParamPart_Ex <SoftwareSerial> Odczyt2(&SerialSoft); // Create object ParamPart_Ex with pointer to Serial as parameter. (SoftwareSerial version)
template <typename SerialType = HardwareSerial>
class ParamPart_Ex final : public ParamPart // Extended Arduino Serial Data Splitter
{

public:
    SerialType * const ptr_Serial; // Pointer to Serial Object (HardwareSerial/SoftwareSerial)

    // Overloaded functions from basic version
    void HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP));
    void Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP));
    String RawRead();
    void readDone(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");
    bool ptrCheck() {if (ptr_Serial) {return true;}; return false;};

    // Constructors for extended class
    ParamPart_Ex(ParamPart_Ex<SerialType> &) = delete; // disable copy constructor

    
    ParamPart_Ex(SerialType *WS, char OL = '<', char DL = ';', char CL = '>') : ParamPart(9, OL, DL, CL), ptr_Serial(WS){};
    ParamPart_Ex(SerialType *WS, int size, char OL = '<', char DL = ';', char CL = '>') : ParamPart(size, OL, DL, CL), ptr_Serial(WS){};
    ~ParamPart_Ex() {};
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
        if (!ptrCheck()) {return ;};

        if (syntaxVerify())
        {                                      //   (SYNTAX OK)
            (M->*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
            if (!(Export_func == nullptr))
                unsetExportFunction();
            if ((m_DebugEnabled) && (m_DebugIntegrityDump != ""))
                ptr_Serial->println(m_DebugIntegrityDump); // Debug Integrity error print (if is ok, nothing to print)
            if ((m_DebugEnabled) && (!Getm_ReadFlag() && (m_DebugIntegrityDump == "")))
                ptr_Serial->println(getError(Errors::UnknownCommand)+" (" + m_Command + ")"); // Unknown command print
            Clear();                                          // Clear parampart to prepare for the next input.
        }
        else
        { // (SYNTAX ERROR)

            if ((m_DebugEnabled))
                ptr_Serial->println(getError(Errors::SyntaxError)); // Syntax Error - missing < or ; or >
        };
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SerialType>
String ParamPart_Ex<SerialType>::RawRead() //Returns line from Serial or nothing
{
    if (!ptrCheck()) {return "";};
    if (ptr_Serial->available())
    {
        m_tmpnewLine = ptr_Serial->readStringUntil(0);
    };

    return (m_tmpnewLine[0] != '#') ? m_tmpnewLine : ""; // If first line is # just return nothing
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename SerialType>
void ParamPart_Ex<SerialType>::readDone(bool RtnMsg, String ParamRtn, String Rtn) // If command code done
{
    if (!ptrCheck()) {return ;};
    if (RtnMsg)
        ptr_Serial->println(m_OpenLine + Rtn + m_DelimiterChar + m_Command + m_DelimiterChar + ParamRtn + m_DelimiterChar + m_CloseLine);
    setReadFlag(true);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DEFAULT INTERPRETER FUNCTION
template <typename SerialType>
void ParamPart_Ex<SerialType>::Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP)) // Start interpret object
{
if (!ptrCheck()) {return ;};
    if (syntaxVerify())
    {                                   //   (SYNTAX OK)
        (*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
        if (!(Export_func == nullptr))
            unsetExportFunction();
        if ((m_DebugEnabled) && (m_DebugIntegrityDump != ""))
            ptr_Serial->println(m_DebugIntegrityDump); // Debug Integrity error print (if is ok, nothing to print)
        if ((m_DebugEnabled) && (!getReadFlag() && (m_DebugIntegrityDump == "")))
            ptr_Serial->println(getError(Errors::UnknownCommand)+" (" + m_Command + ")"); // Unknown command print
        Clear();                                            // Clear parampart to prepare for the next input.
    }
    else
    { // (SYNTAX ERROR)

        if ((m_DebugEnabled))
            ptr_Serial->println(getError(Errors::SyntaxError)); // Syntax Error - missing < or ; or >
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename SerialType>
void ParamPart_Ex<SerialType>::HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex<SerialType> &PP)) // Receive Serial line and Start interpret object.
{
    if (!ptrCheck()) {return;};
    RawRead();

    if ((m_tmpnewLine[0] != '#') && (m_tmpnewLine != ""))
    {
        Slicer(m_tmpnewLine);
        Interpreter(ptn_func_interpreter);
    };
};




#pragma once
#include "parampart_ex.h"

/* 
Arduino Serial String Data Splitter  - ParamPart_Ex (Extended Version with Serial Receiver)
Written by Piotr K. (dajmosster@gmail.com / piotrq.eu) 
2019 - 2021
v. 3.6
*/
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
            ptr_Serial->println("UC! (" + m_Command + ")"); // Unknown command print
        Clear();                                            // Clear parampart to prepare for the next input.
    }
    else
    { // (SYNTAX ERROR)

        if ((m_DebugEnabled))
            ptr_Serial->println("SE!"); // Syntax Error - missing < or ; or >
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

#include "parampart_ex.h"

/* 
Arduino Serial String Data Splitter  - ParamPart_Ex (Extended Version with Serial Receiver)
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 3.4.2
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename SerialType>
String ParamPart_Ex<SerialType>::RawRead() //Returns line from Serial or nothing
{

    if (pnt_Serial->available())
    {
        tmpnewLine = pnt_Serial->readStringUntil(0);
    };

    return (tmpnewLine[0] != '#') ? tmpnewLine : ""; // If first line is # just return nothing
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename SerialType>
void ParamPart_Ex<SerialType>::ReadDone(bool RtnMsg, String ParamRtn, String Rtn) // If command code done
{
    if (RtnMsg)
        pnt_Serial->println(OpenLine + Rtn + DelimiterChar + Command + DelimiterChar + ParamRtn + DelimiterChar + CloseLine);
    SetReadFlag(true);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename SerialType>
void ParamPart_Ex<SerialType>::Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex &PP)) // Start interpret object
{

    if (SyntaxVerify())
    {                                   //   (SYNTAX OK)
        (*ptn_func_interpreter)(*this); // Execute reaction function (callback), push pointer of this class to access from external function.
        if (!(Export_func == nullptr))
            UnSetExportFunction();
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

template <typename SerialType>
void ParamPart_Ex<SerialType>::HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex &PP)) // Receive Serial line and Start interpret object.
{
    RawRead();

    if ((tmpnewLine[0] != '#') && (tmpnewLine != ""))
    {
        Slicer(tmpnewLine);
        Interpreter(ptn_func_interpreter);
    };
};

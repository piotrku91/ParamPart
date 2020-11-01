#include "parampart_ex.h"

/* 
Arduino Serial Data Splitter - ParamPart_Ex (Extended Version with Serial Receiver)
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////

String ParamPart_Ex::RawRead() //Returns line from Serial or nothing
{
    String tmpnewLine;

    while (pnt_Serial->available())
    {
        tmpnewLine = pnt_Serial->readStringUntil(0);
    };

    if (tmpnewLine[0] != '#') // If first line is # just return nothing
    {
        return tmpnewLine;
    }
    else
    {
        return "";
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart_Ex::Readed(bool RtnMsg, String ParamRtn, String Rtn) // If command code done
{
    if (RtnMsg)
        pnt_Serial->println(OpenLine + Rtn + DelimiterChar + Command + DelimiterChar + ParamRtn + DelimiterChar + CloseLine);
    SetReadFlag(true);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ParamPart_Ex::Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex *PP)) // Start interpret object
{

    if (SyntaxVerify())
    {                                  //   (SYNTAX OK)
        (*ptn_func_interpreter)(this); // Execute reaction function (callback), push pointer of this class to access from external function.

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

void ParamPart_Ex::HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex *PP)) // Receive Serial line and Start interpret object.
{
    String tmpnewLine = RawRead();
    if ((tmpnewLine[0] != '#') && (tmpnewLine != "")) 
    {
        Slicer(&tmpnewLine);
        Interpreter(ptn_func_interpreter);
    };
};

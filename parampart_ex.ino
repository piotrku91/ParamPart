#include "parampart_ex.h";

/* 
Arduino Serial Data Splitter - ParamPart_Ex (Extended Version with Serial Receiver)
Written by Piotr Kupczyk (dajmosster@gmail.com) 
2019 - 2020
v. 2.4
*/

void ParamPart_Ex::SerialChecker(char newLine[])
{
    String Dane; // tmp string

    strcpy(newLine, "#"); // Lines starts with # char are ignored.

    while (pnt_Serial->available())
    {
        Dane = pnt_Serial->readStringUntil(NULL);
        strcpy(newLine, Dane.c_str());
    };
};

String ParamPart_Ex::RawRead()
{
    char tmpnewLine[64];
    SerialChecker(tmpnewLine);
    if (tmpnewLine[0] != '#')
    {
        return tmpnewLine;
    }
    else
    {
        return "";
    };
};

void ParamPart_Ex::HybridRead(void (*ptn_func_interpreter)(ParamPart_Ex *PP))
{
    char tmpnewLine[64];
    SerialChecker(tmpnewLine);

    if (tmpnewLine[0] != '#')
    {
        Slicer(tmpnewLine);

        if (SyntaxVerify())
        { //   (SYNTAX OK)

            (*ptn_func_interpreter)(this); // Execute reaction function (callback), push pointer of this class to access from external function.

            if ((DebugEnabled) && (DebugIntegrityDump != ""))
                pnt_Serial->println(DebugIntegrityDump); // Debug Integrity print
            if ((DebugEnabled) && (!GetReadFlag()))
                pnt_Serial->println("UC!"); // Unknown command print
            Clear();                        // Clear parampart to prepare for the next input.
        }
        else
        { // (SYNTAX ERROR)

            if ((DebugEnabled))
                pnt_Serial->println("SE!"); // Syntax Error - missing < or ; or >
        };
    }
};
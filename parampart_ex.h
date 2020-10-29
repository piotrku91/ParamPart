#ifndef PARAMPART_EX_H
#define PARAMPART_EX_H
#include "parampart.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamParted_Ex  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParamPart_Ex : public ParamPart
{
public:
    HardwareSerial *pnt_Serial;

    void HybridRead(void (*ptn_func_interpreter)(ParamPart_Ex *PP));
    String RawRead();
    void SerialChecker(String *newLine);
    void Readed(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");

    // Constructors for extended class
    ParamPart_Ex(HardwareSerial *WS) : ParamPart(), pnt_Serial(WS){};
    ParamPart_Ex(HardwareSerial *WS, char OL, char DC, char CL) : ParamPart(OL, DC, CL), pnt_Serial(WS){};
};

#endif

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
    String ParamPart_Ex::RawRead();
    void SerialChecker(char newLine[]);
    
    // Constructors for extended class
    ParamPart_Ex(HardwareSerial *WS) : ParamPart(), pnt_Serial(WS){};
};

#endif PARAMPART_EX__H

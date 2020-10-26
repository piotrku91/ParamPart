#ifndef PARAMPART_EX_H
#define PARAMPART_EX_H

#include "parampart.h"

class ParamPart_Ex : public ParamPart
{
public:
    HardwareSerial *pnt_Serial;

    void HybridRead(void (*ptn_func_interpreter)(ParamPart_Ex *PP));

    void ParamPart_Ex::SerialChecker(char newLine[]);

    // Konstruktor dla rozbudowanej klasy
    ParamPart_Ex(HardwareSerial *WS) : ParamPart(), pnt_Serial(WS){};
};

#endif PARAMPART_EX__H

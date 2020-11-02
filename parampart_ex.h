#ifndef PARAMPART_EX_H
#define PARAMPART_EX_H
#include "parampart.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        CLASS ParamParted_Ex  - Header file                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ParamPart_Ex : public ParamPart // Extended Arduino Serial Data Splitter
{
public:
    HardwareSerial *pnt_Serial;

    void HybridInterpreter(void (*ptn_func_interpreter)(ParamPart_Ex& PP));
    void Interpreter(void (*ptn_func_interpreter)(ParamPart_Ex& PP));
    String RawRead();
    void Readed(bool RtnMsg = true, String ParamRtn = "OK", String Rtn = "artn");

    // Constructors for extended class
    ParamPart_Ex() = delete; // disable default constructor
    ParamPart_Ex(HardwareSerial *WS, char OL = '<', char DL = ';', char CL = '>') : 
    ParamPart(OL, DL, CL), pnt_Serial(WS){};
};

#endif
